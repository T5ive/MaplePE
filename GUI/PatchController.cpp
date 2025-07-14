#include "PatchView.h"
#include "PatchController.h"
#include "Injector.h"


#include "Common.h"
#ifdef _DEBUG
#pragma comment(lib, "CommonD.lib")
#else
#pragma comment(lib, "Common.lib")
#endif


PatchController::PatchController(IMainController* mainControllerImpl, PatchView* view)
{
	m_mainControllerImpl = mainControllerImpl;
	m_patchView = view;
	const Setting setting = mainControllerImpl->GetSetting();
	std::wstring exeDir = m_mainControllerImpl->GetExeDir();
	m_dllPath = exeDir + L"\\" + setting.PacketDllName;
}

PatchController::~PatchController()
{
}

DWORD PatchController::FindProcessID()
{
	const Setting setting = m_mainControllerImpl->GetSetting();
	return Injector::FindProcessIDByName(setting.GameProcessName, setting.PacketDllName);
}

std::wstring PatchController::InjectDLL(DWORD pid)
{
	const Setting setting = m_mainControllerImpl->GetSetting();
	if (CheckEmptyAddr(setting)) {
		return L"Some function addresses are missing.\r\nPlease fill them in setting view from the top-left menu";
	}
	if (Injector::HasModule(pid, setting.PacketDllName)) {
		return L"The target process has injected dll";
	}
	if (!Injector::InjectDll(pid, m_dllPath)) {
		return L"Failed to inject dll";
	}
	return std::wstring();
}