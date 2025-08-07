#pragma once
#include <windows.h>
#include <vector>
#include <string>

namespace {
	const UINT kSystemAnsiCodePage = GetACP();
}

namespace PacketScript {

	const size_t kCharacterNameLength = 13;
	const size_t kFileTimeLength = 8;

	void InitHexStream(std::wstringstream& ss);

	std::wstring Int2Hex(uint32_t v);

	void Buffer2Data(std::vector<uint8_t>& buffer, std::wstring& data);

	bool Data2Buffer(const std::wstring& data, std::vector<uint8_t>& buffer);

	std::wstring FormatSystemTime(const SYSTEMTIME& st);

	SYSTEMTIME ParseSystemTime(const std::wstring& timeStr);

	std::wstring MultiByte2WideChar(const char* pStr, size_t uSize);

	std::string WideChar2MultiByte(const std::wstring& wstr);

	bool IsTimeValid(const SYSTEMTIME& st);

	std::wstring GetHexSegment(const std::wstring& data, size_t& segmentPos, size_t uSize);

	uint8_t Decode1(const std::vector<uint8_t>& buffer, size_t& pos);

	uint16_t Decode2(const std::vector<uint8_t>& buffer, size_t& pos);

	uint32_t Decode4(const std::vector<uint8_t>& buffer, size_t& pos);

	uint64_t Decode8(const std::vector<uint8_t>& buffer, size_t& pos);

	SYSTEMTIME DecodeFT(const std::vector<uint8_t>& buffer, size_t& pos, uint64_t& value);

	std::wstring DecodeStr(const std::vector<uint8_t>& buffer, size_t& pos, size_t& uSize);

	std::wstring DecodeBuffer(const std::vector<uint8_t>& buffer, size_t& pos, size_t uSize);

	void Encode1(std::vector<uint8_t>& buffer, uint8_t value);

	void Encode2(std::vector<uint8_t>& buffer, uint16_t value);

	void Encode4(std::vector<uint8_t>& buffer, uint32_t value);

	void Encode8(std::vector<uint8_t>& buffer, uint64_t value);

	void EncodeFT(std::vector<uint8_t>& buffer, const std::wstring& timeStr);

	void EncodeStr(std::vector<uint8_t>& buffer, const std::wstring& wstr, size_t& uSize);

	void EncodeBuffer(std::vector<uint8_t>& buffer, const std::wstring& data, size_t uSize);
}
