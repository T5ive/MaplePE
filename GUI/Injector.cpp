#include "pch.h"
#include "Injector.h"

namespace {

	typedef HMODULE(WINAPI* pLoadLibraryW)(_In_ LPCWSTR lpLibFileName);
	typedef HANDLE(WINAPI* pCreateRemoteThread)(
		_In_ HANDLE hProcess,
		_In_opt_ LPSECURITY_ATTRIBUTES lpThreadAttributes,
		_In_ SIZE_T dwStackSize,
		_In_ LPTHREAD_START_ROUTINE lpStartAddress,
		_In_opt_ LPVOID lpParameter,
		_In_ DWORD dwCreationFlags,
		_Out_opt_ LPDWORD lpThreadId
		);
	static pLoadLibraryW fnLoadLibraryW = nullptr;
	static pCreateRemoteThread fnCreateRemoteThread = nullptr;

}

namespace Injector {

	// Initialize dynamically resolved API functions (LoadLibraryW and CreateRemoteThread)
	bool Init() {
		if (fnLoadLibraryW != nullptr && fnCreateRemoteThread != nullptr) {
			return true;
		}
		HMODULE hKernel32 = GetModuleHandleA("kernel32.dll");
		if (hKernel32 == nullptr) {
			return false;
		}
		fnLoadLibraryW = (pLoadLibraryW)GetProcAddress(hKernel32, "LoadLibraryW");
		fnCreateRemoteThread = (pCreateRemoteThread)GetProcAddress(hKernel32, "CreateRemoteThread");
		return (fnLoadLibraryW != nullptr && fnCreateRemoteThread != nullptr);
	}

	bool HasModule(DWORD pid, const std::wstring& targetModuleName) {
		HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
		if (hSnapshot == INVALID_HANDLE_VALUE) return false;

		MODULEENTRY32W me = { sizeof(me) };

		if (Module32FirstW(hSnapshot, &me)) {
			do {
				if (_wcsicmp(me.szModule, targetModuleName.c_str()) == 0) {
					CloseHandle(hSnapshot);
					return true;
				}
			} while (Module32NextW(hSnapshot, &me));
		}

		CloseHandle(hSnapshot);
		return false;
	}

	DWORD FindProcessIDByName(const std::wstring& processName, const std::wstring& dllName)
	{
		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		if (snapshot == INVALID_HANDLE_VALUE) {
			return 0;
		}
		PROCESSENTRY32W pe{};
		pe.dwSize = sizeof(pe);
		DWORD pid = 0;
		if (Process32FirstW(snapshot, &pe)) {
			do {
				if (pe.szExeFile == processName && !HasModule(pe.th32ProcessID, dllName)) {
					pid = pe.th32ProcessID;
					break;
				}
			} while (Process32NextW(snapshot, &pe));
		}
		CloseHandle(snapshot);
		return pid;
	}

	bool InjectDll(int pid, std::wstring dllPath)
	{
		if (!Init()) {
			return false;
		}
		// Open the target process with full access rights
		HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		if (!hProcess) {
			return false;
		}
		// Allocate memory in the target process for the DLL path
		LPVOID pRemoteDllPath = VirtualAllocEx(hProcess, nullptr, (dllPath.size() + 1) * sizeof(wchar_t), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
		if (!pRemoteDllPath) {
			CloseHandle(hProcess);
			return false;
		}
		// Write the DLL path into the allocated memory in the target process
		if (!WriteProcessMemory(hProcess, pRemoteDllPath, dllPath.c_str(), (dllPath.size() + 1) * sizeof(wchar_t), nullptr)) {
			VirtualFreeEx(hProcess, pRemoteDllPath, 0, MEM_RELEASE);
			CloseHandle(hProcess);
			return false;
		}
		// Create a remote thread in the target process to call LoadLibraryW with the DLL path
		HANDLE hThread = fnCreateRemoteThread(hProcess, nullptr, 0, (LPTHREAD_START_ROUTINE)fnLoadLibraryW, pRemoteDllPath, 0, nullptr);
		if (!hThread) {
			VirtualFreeEx(hProcess, pRemoteDllPath, 0, MEM_RELEASE);
			CloseHandle(hProcess);
			return false;
		}
		// Wait for the remote thread to finish
		WaitForSingleObject(hThread, INFINITE);

		// Free allocated memory and close handles
		VirtualFreeEx(hProcess, pRemoteDllPath, 0, MEM_RELEASE);
		CloseHandle(hThread);
		CloseHandle(hProcess);
		return true;
	}
}