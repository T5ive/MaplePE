#pragma once
#include <windows.h>
#include <string>

namespace Injector {
	bool Init();
	bool HasModule(DWORD pid, const std::wstring& targetModuleName);
	DWORD FindProcessIDByName(const std::wstring& processName, const std::wstring& dllName);
	bool InjectDll(int pid, std::wstring dllPath);
}