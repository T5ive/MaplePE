#pragma once
#include <windows.h>
#include <string>

std::wstring BYTE2String(BYTE b);
std::wstring DWORD2String(DWORD dw);
std::wstring Str2WStr(const std::string& str);
ULONG_PTR GetFuncAddress(ULONG_PTR uCall);
ULONG_PTR GetDWORDAddress(ULONG_PTR uPush);
#ifdef _DEBUG
#define DEBUGW(msg) \
	{\
		std::wstring wmsg = L"[Debug] ";\
		wmsg += msg;\
		OutputDebugStringW(wmsg.c_str());\
	}
#else
#define DEBUGW(msg)
#endif


#define SCANRES(msg) { DEBUGW(L""#msg" = " + DWORD2String(msg));}