#pragma once
#include <Windows.h>
#include <string>
#include <fstream>

inline bool WSTR2BOOL(const std::wstring& value) {
	return (!value.empty() && value != L"0");
}

inline ULONG_PTR WSTR2ULONGPTR(const std::wstring& value, ULONG_PTR defaultValue = 0) {
	try {
		return static_cast<ULONG_PTR>(std::stoull(value, nullptr, 0));
	}
	catch (...) {
		return defaultValue;
	}
}

#define SET_BOOL_FIELD(name) \
    else if (key == L#name) s.name = WSTR2BOOL(value);

#define SET_ADDR_FIELD(name) \
    else if (key == L#name) s.name = WSTR2ULONGPTR(value);

#define SET_WSTR_FIELD(name) \
    else if (key == L#name) s.name = value;

const std::wstring kSettingFilename = L"Setting.txt";

struct Setting {
	// Base info
	std::wstring GameProcessName = L"MapleStory.exe";
	std::wstring PacketDllName = L"Packet.dll";
	std::wstring LoggingServerIP = L"127.0.0.1";
	ULONG_PTR LoggingServerPort = 0xC0DE;
	bool IsTypeHeader1Byte = false;

	// Function Addr
	ULONG_PTR CInPacketDecode1Addr = 0x00000000;
	ULONG_PTR CInPacketDecode2Addr = 0x00000000;
	ULONG_PTR CInPacketDecode4Addr = 0x00000000;
	ULONG_PTR CInPacketDecode8Addr = 0x00000000;
	ULONG_PTR CInPacketDecodeStrAddr = 0x00000000;
	ULONG_PTR CInPacketDecodeBufferAddr = 0x00000000;

	ULONG_PTR COutPacketEncode1Addr = 0x00000000;
	ULONG_PTR COutPacketEncode2Addr = 0x00000000;
	ULONG_PTR COutPacketEncode4Addr = 0x00000000;
	ULONG_PTR COutPacketEncode8Addr = 0x00000000;
	ULONG_PTR COutPacketEncodeStrAddr = 0x00000000;
	ULONG_PTR COutPacketEncodeBufferAddr = 0x00000000;
	ULONG_PTR COutPacketMakeBufferListAddr = 0x00000000;

	ULONG_PTR CClientSocketProcessPacketAddr = 0x00000000;
	ULONG_PTR CClientSocketSendPacketAddr = 0x00000000;
	// Template code
	std::wstring CInPacketDecode1GenCode = L"v%d := iPacket.Decode1()";
	std::wstring CInPacketDecode2GenCode = L"v%d := iPacket.Decode2()";
	std::wstring CInPacketDecode4GenCode = L"v%d := iPacket.Decode4()";
	std::wstring CInPacketDecode8GenCode = L"v%d := iPacket.Decode8()";
	std::wstring CInPacketDecodeStrGenCode = L"str%d := iPacket.DecodeStr()";
	std::wstring CInPacketDecodeBufferGenCode = L"buf%d := iPacket.DecodeBuffer(%d)";

	std::wstring COutPacketEncode1GenCode = L"oPacket.Encode1(%ls)";
	std::wstring COutPacketEncode2GenCode = L"oPacket.Encode2(%ls)";
	std::wstring COutPacketEncode4GenCode = L"oPacket.Encode4(%ls)";
	std::wstring COutPacketEncode8GenCode = L"oPacket.Encode8(%ls)";
	std::wstring COutPacketEncodeStrGenCode = L"oPacket.EncodeStr(\"%ls\")";
	std::wstring COutPacketEncodeBufferGenCode = L"oPacket.EncodeBuffer(make([]byte,%d))";
};

static std::wstring GetExecutableDir(HMODULE hModule) {
	WCHAR path[MAX_PATH];
	GetModuleFileNameW(hModule, path, MAX_PATH);
	std::wstring executablePath = std::wstring(path);
	size_t pos = executablePath.find_last_of(L"\\/");
	return (std::wstring::npos == pos) ? L"" : executablePath.substr(0, pos);
}

static bool CheckEmptyAddr(const Setting& s) {
	return s.CInPacketDecode1Addr == 0 ||
		s.CInPacketDecode2Addr == 0 ||
		s.CInPacketDecode4Addr == 0 ||
		s.CInPacketDecodeStrAddr == 0 ||
		s.CInPacketDecodeBufferAddr == 0 ||
		s.COutPacketEncode1Addr == 0 ||
		s.COutPacketEncode2Addr == 0 ||
		s.COutPacketEncode4Addr == 0 ||
		s.COutPacketEncodeStrAddr == 0 ||
		s.COutPacketEncodeBufferAddr == 0 ||
		s.COutPacketMakeBufferListAddr == 0 ||
		s.CClientSocketProcessPacketAddr == 0 ||
		s.CClientSocketSendPacketAddr == 0;
}

static bool LoadSetting(std::wstring dir, Setting& s) {
	std::wstring path = dir + L"\\" + kSettingFilename;
	std::wifstream wfile(path);
	if (!wfile && !wfile.is_open()) {
		return false;
	}
	std::wstring wline;
	while (std::getline(wfile, wline)) {
		size_t pos = wline.find(L'=');
		if (pos == std::wstring::npos) {
			continue;
		};

		std::wstring key = wline.substr(0, pos);
		std::wstring value = wline.substr(pos + 1);

		if (false);
		// Base info
		SET_WSTR_FIELD(GameProcessName)
		SET_WSTR_FIELD(PacketDllName)
		SET_WSTR_FIELD(LoggingServerIP)
		SET_ADDR_FIELD(LoggingServerPort)
		SET_BOOL_FIELD(IsTypeHeader1Byte)
		// Function Addr
		SET_ADDR_FIELD(CInPacketDecode1Addr)
		SET_ADDR_FIELD(CInPacketDecode2Addr)
		SET_ADDR_FIELD(CInPacketDecode4Addr)
		SET_ADDR_FIELD(CInPacketDecode8Addr)
		SET_ADDR_FIELD(CInPacketDecodeStrAddr)
		SET_ADDR_FIELD(CInPacketDecodeBufferAddr)
		SET_ADDR_FIELD(COutPacketEncode1Addr)
		SET_ADDR_FIELD(COutPacketEncode2Addr)
		SET_ADDR_FIELD(COutPacketEncode4Addr)
		SET_ADDR_FIELD(COutPacketEncode8Addr)
		SET_ADDR_FIELD(COutPacketEncodeStrAddr)
		SET_ADDR_FIELD(COutPacketEncodeBufferAddr)
		SET_ADDR_FIELD(COutPacketMakeBufferListAddr)
		SET_ADDR_FIELD(CClientSocketProcessPacketAddr)
		SET_ADDR_FIELD(CClientSocketSendPacketAddr)
		// Template code
		SET_WSTR_FIELD(CInPacketDecode1GenCode)
		SET_WSTR_FIELD(CInPacketDecode2GenCode)
		SET_WSTR_FIELD(CInPacketDecode4GenCode)
		SET_WSTR_FIELD(CInPacketDecode8GenCode)
		SET_WSTR_FIELD(CInPacketDecodeStrGenCode)
		SET_WSTR_FIELD(CInPacketDecodeBufferGenCode)
		SET_WSTR_FIELD(COutPacketEncode1GenCode)
		SET_WSTR_FIELD(COutPacketEncode2GenCode)
		SET_WSTR_FIELD(COutPacketEncode4GenCode)
		SET_WSTR_FIELD(COutPacketEncode8GenCode)
		SET_WSTR_FIELD(COutPacketEncodeStrGenCode)
		SET_WSTR_FIELD(COutPacketEncodeBufferGenCode)
	}
	return true;
}

static bool SaveSetting(std::wstring dir, const Setting& s) {
	std::wstring path = dir + L"\\" + kSettingFilename;
	std::wofstream wfile(path);
	if (!wfile) {
		return false;
	}
	// Base info
	wfile << L"GameProcessName=" << s.GameProcessName << L"\n";
	wfile << L"PacketDllName=" << s.PacketDllName << L"\n";
	wfile << L"LoggingServerIP=" << s.LoggingServerIP << L"\n";
	wfile << L"LoggingServerPort=" << s.LoggingServerPort << L"\n";
	wfile << L"IsTypeHeader1Byte=" << s.IsTypeHeader1Byte << L"\n";
	// Function Addr
	wfile << L"CInPacketDecode1Addr=0x" << std::hex << s.CInPacketDecode1Addr << L"\n";
	wfile << L"CInPacketDecode2Addr=0x" << std::hex << s.CInPacketDecode2Addr << L"\n";
	wfile << L"CInPacketDecode4Addr=0x" << std::hex << s.CInPacketDecode4Addr << L"\n";
	wfile << L"CInPacketDecode8Addr=0x" << std::hex << s.CInPacketDecode8Addr << L"\n";
	wfile << L"CInPacketDecodeStrAddr=0x" << std::hex << s.CInPacketDecodeStrAddr << L"\n";
	wfile << L"CInPacketDecodeBufferAddr=0x" << std::hex << s.CInPacketDecodeBufferAddr << L"\n";
	wfile << L"COutPacketEncode1Addr=0x" << std::hex << s.COutPacketEncode1Addr << L"\n";
	wfile << L"COutPacketEncode2Addr=0x" << std::hex << s.COutPacketEncode2Addr << L"\n";
	wfile << L"COutPacketEncode4Addr=0x" << std::hex << s.COutPacketEncode4Addr << L"\n";
	wfile << L"COutPacketEncode8Addr=0x" << std::hex << s.COutPacketEncode8Addr << L"\n";
	wfile << L"COutPacketEncodeStrAddr=0x" << std::hex << s.COutPacketEncodeStrAddr << L"\n";
	wfile << L"COutPacketEncodeBufferAddr=0x" << std::hex << s.COutPacketEncodeBufferAddr << L"\n";
	wfile << L"COutPacketMakeBufferListAddr=0x" << std::hex << s.COutPacketMakeBufferListAddr << L"\n";
	wfile << L"CClientSocketProcessPacketAddr=0x" << std::hex << s.CClientSocketProcessPacketAddr << L"\n";
	wfile << L"CClientSocketSendPacketAddr=0x" << std::hex << s.CClientSocketSendPacketAddr << L"\n";
	// Template code
	wfile << L"CInPacketDecode1GenCode=" << s.CInPacketDecode1GenCode << L"\n";
	wfile << L"CInPacketDecode2GenCode=" << s.CInPacketDecode2GenCode << L"\n";
	wfile << L"CInPacketDecode4GenCode=" << s.CInPacketDecode4GenCode << L"\n";
	wfile << L"CInPacketDecode8GenCode=" << s.CInPacketDecode8GenCode << L"\n";
	wfile << L"CInPacketDecodeStrGenCode=" << s.CInPacketDecodeStrGenCode << L"\n";
	wfile << L"CInPacketDecodeBufferGenCode=" << s.CInPacketDecodeBufferGenCode << L"\n";
	wfile << L"COutPacketEncode1GenCode=" << s.COutPacketEncode1GenCode << L"\n";
	wfile << L"COutPacketEncode2GenCode=" << s.COutPacketEncode2GenCode << L"\n";
	wfile << L"COutPacketEncode4GenCode=" << s.COutPacketEncode4GenCode << L"\n";
	wfile << L"COutPacketEncode8GenCode=" << s.COutPacketEncode8GenCode << L"\n";
	wfile << L"COutPacketEncodeStrGenCode=" << s.COutPacketEncodeStrGenCode << L"\n";
	wfile << L"COutPacketEncodeBufferGenCode=" << s.COutPacketEncodeBufferGenCode << L"\n";

	wfile.flush();
	return true;
}

