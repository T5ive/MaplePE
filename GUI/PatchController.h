#pragma once
#include "IMainController.h"

class PatchView;

class PatchController final {
public:
	PatchController(IMainController* mainControllerImpl, PatchView* view);
	DWORD FindProcessID();
	std::wstring InjectDLL(DWORD pid);
	~PatchController();
private:
	std::wstring m_dllPath;
	PatchView* m_patchView = nullptr;
	IMainController* m_mainControllerImpl = nullptr;
};