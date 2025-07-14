#pragma once
#include <Windows.h>
#include <vector>
#include <sstream>
#include <iomanip>

namespace {
	const UINT kSystemAnsiCodePage = GetACP();
}

namespace PacketScript {

	const size_t kCharacterNameLength = 13;
	const size_t kFileTimeLength = 8;

	static void InitHexStream(std::wstringstream& ss) {
		ss << std::uppercase << std::hex << std::setfill(L'0');
	}

	static std::wstring Int2Hex(uint32_t v) {
		std::wstringstream ss;
		InitHexStream(ss);
		ss << L"0x" << std::setw(8) << v;
		return ss.str();
	}

	static void Buffer2Data(std::vector<uint8_t>& buffer, std::wstring& data) {
		std::wstringstream ss;
		InitHexStream(ss);

		for (size_t i = 0; i < buffer.size(); ++i) {
			if (i > 0) {
				ss << L' ';
			}
			ss << std::setw(2) << static_cast<int>(buffer[i]);
		}
		data = ss.str();
	}

	static bool Data2Buffer(const std::wstring& data, std::vector<uint8_t>& buffer) {
		buffer.clear();
		std::wstringstream ss(data);
		std::wstring byteStr;

		while (ss >> byteStr) {
			try {
				int byteVal = std::stoul(byteStr, nullptr, 16);
				if (byteVal <= 0xFF) {
					buffer.push_back(static_cast<uint8_t>(byteVal));
				}
			}
			catch (const std::exception&) {
				return false;
			}
		}
		return true;
	}

	static std::wstring FormatSystemTime(const SYSTEMTIME& st) {
		std::wstringstream ss;
		ss << std::setfill(L'0') << std::setw(4) << st.wYear << L"-"
			<< std::setw(2) << st.wMonth << L"-"
			<< std::setw(2) << st.wDay << L" "
			<< std::setw(2) << st.wHour << L":"
			<< std::setw(2) << st.wMinute << L":"
			<< std::setw(2) << st.wSecond;
		return ss.str();
	}

	static SYSTEMTIME ParseSystemTime(const std::wstring& timeStr) {
		SYSTEMTIME st{};
		swscanf_s(timeStr.c_str(), L"%4hu-%2hu-%2hu %2hu:%2hu:%2hu",
			&st.wYear, &st.wMonth, &st.wDay,
			&st.wHour, &st.wMinute, &st.wSecond);
		return st;
	}

	static std::wstring MultiByte2WideChar(const char* pStr, size_t uSize) {
		int wcharLen = MultiByteToWideChar(kSystemAnsiCodePage, 0, pStr, uSize, nullptr, 0);
		if (wcharLen <= 0) {
			return std::wstring();
		}
		std::wstring result(wcharLen, 0);
		MultiByteToWideChar(kSystemAnsiCodePage, 0, pStr, uSize, &result[0], wcharLen);
		return result;
	}

	static std::string WideChar2MultiByte(const std::wstring& wstr) {
		int byteLen = WideCharToMultiByte(kSystemAnsiCodePage, 0, wstr.c_str(), wstr.size(), nullptr, 0, nullptr, nullptr);
		if (byteLen <= 0) {
			return std::string();
		}
		std::string result(byteLen, 0);
		WideCharToMultiByte(kSystemAnsiCodePage, 0, wstr.c_str(), wstr.size(), &result[0], byteLen, nullptr, nullptr);
		return result;
	}

	static bool IsTimeValid(const SYSTEMTIME& st) {
		// Valid if epoch, fallback default, or within 21st century range
		// System Time 0000-00-00 00:00:00 <-> FileTime 1601-01-01 00:00:00 
		// System Time 0000-00-00 00:00:00 <-> FileTime 1601-01-03 00:37:51
		// System Time 0001-01-01 00:00:00 <-> FileTime 1754-08-30 22:43:41 
		bool isZeroTime = st.wYear == 1601 && st.wMonth == 1 && st.wDay == 1 && st.wHour == 0 && st.wMinute == 0 && st.wSecond == 0;
		bool isMinimalTime = st.wYear == 1601 && st.wMonth == 1 && st.wDay == 3 && st.wHour == 0 && st.wMinute == 37 && st.wSecond == 51;
		bool isFallbackTime = st.wYear == 1754 && st.wMonth == 8 && st.wDay == 30 && st.wHour == 22 && st.wMinute == 43 && st.wSecond == 41;
		bool isValidTime = st.wYear >= 2000 && st.wYear <= 2100 && st.wMonth >= 1 && st.wMonth <= 12 && st.wDay >= 1 && st.wDay <= 31;
		return isZeroTime || isMinimalTime || isFallbackTime || isValidTime;
	}

	static std::wstring GetHexSegment(const std::wstring& data, size_t& segmentPos, size_t uSize) {
		size_t segmentLen = uSize * 3;
		if (segmentPos >= data.length()) {
			return std::wstring();
		}
		std::wstring segment;
		size_t remainLen = data.length() - segmentPos;
		size_t extractLen = (segmentLen < remainLen) ? segmentLen : remainLen;
		segment = data.substr(segmentPos, extractLen);
		segmentPos += extractLen;
		return segment;
	}

	static uint8_t Decode1(const std::vector<uint8_t>& buffer, size_t& pos) {
		if (pos + 1 > buffer.size()) {
			return 0;
		}
		uint8_t value = buffer[pos];
		pos++;
		return value;
	}

	static uint16_t Decode2(const std::vector<uint8_t>& buffer, size_t& pos) {
		if (pos + 2 > buffer.size()) {
			return 0;
		}
		uint16_t value = static_cast<uint16_t>(buffer[pos] | (buffer[pos + 1] << 8));
		pos += 2;
		return value;
	}

	static uint32_t Decode4(const std::vector<uint8_t>& buffer, size_t& pos) {
		if (pos + 4 > buffer.size()) {
			return 0;
		}
		uint32_t value = 0;
		for (size_t i = 0; i < 4; i++)
		{
			value |= static_cast<uint32_t>(buffer[pos + i]) << (i * 8);
		}
		pos += 4;
		return value;
	}

	static uint64_t Decode8(const std::vector<uint8_t>& buffer, size_t& pos) {
		if (pos + 8 > buffer.size()) {
			return 0;
		}
		uint64_t value = 0;
		for (size_t i = 0; i < 8; i++)
		{
			value |= static_cast<uint64_t>(buffer[pos + i]) << (i * 8);
		}
		pos += 8;
		return value;
	}

	static SYSTEMTIME DecodeFT(const std::vector<uint8_t>& buffer, size_t& pos, uint64_t& value) {
		value = Decode8(buffer, pos);
		FILETIME ft{};
		SYSTEMTIME st{};
		ft.dwLowDateTime = static_cast<uint32_t>(value & 0xFFFFFFFF);
		ft.dwHighDateTime = static_cast<uint32_t>(value >> 32);
		if (FileTimeToSystemTime(&ft, &st)) {
			return st;
		}
		return {};
	}

	static std::wstring DecodeStr(const std::vector<uint8_t>& buffer, size_t& pos, size_t& uSize) {
		uSize = Decode2(buffer, pos);
		if (uSize == 0 || pos + uSize > buffer.size()) {
			return std::wstring();
		}
		const char* pStr = reinterpret_cast<const char*>(&buffer[pos]);
		std::wstring wstr = MultiByte2WideChar(pStr, uSize);
		pos += uSize;
		return wstr;
	}

	static std::wstring DecodeBuffer(const std::vector<uint8_t>& buffer, size_t& pos, size_t uSize) {
		if (pos + uSize > buffer.size()) {
			return std::wstring();
		}
		switch (uSize) {
		case kCharacterNameLength: {
			const char* pStr = reinterpret_cast<const char*>(&buffer[pos]);
			std::wstring wStr = MultiByte2WideChar(pStr, kCharacterNameLength);
			pos += kCharacterNameLength;
			return wStr;
		}
		case kFileTimeLength: {
			uint64_t value = 0;
			SYSTEMTIME st = DecodeFT(buffer, pos, value);
			return IsTimeValid(st) ? FormatSystemTime(st) : std::to_wstring(static_cast<int64_t>(value));
		}
		default:
			uint8_t firstValue = buffer[pos];
			bool isDifferent = false;
			for (size_t i = pos; i < pos + uSize; i++)
			{
				if (buffer[i] != firstValue) {
					isDifferent = true;
					break;
				}
			}
			pos += uSize;
			return isDifferent ? std::wstring() : std::to_wstring(static_cast<int8_t>(firstValue));
		}
	}

	static void Encode1(std::vector<uint8_t>& buffer, uint8_t value) {
		buffer.push_back(value);
	}

	static void Encode2(std::vector<uint8_t>& buffer, uint16_t value) {
		const uint8_t* p = reinterpret_cast<const uint8_t*>(&value);
		buffer.insert(buffer.end(), p, p + 2);
	}

	static void Encode4(std::vector<uint8_t>& buffer, uint32_t value) {
		const uint8_t* p = reinterpret_cast<const uint8_t*>(&value);
		buffer.insert(buffer.end(), p, p + 4);
	}

	static void Encode8(std::vector<uint8_t>& buffer, uint64_t value) {
		const uint8_t* p = reinterpret_cast<const uint8_t*>(&value);
		buffer.insert(buffer.end(), p, p + 8);
	}

	static void EncodeFT(std::vector<uint8_t>& buffer, const std::wstring& timeStr) {
		FILETIME ft{};
		SYSTEMTIME st = ParseSystemTime(timeStr);
		if (IsTimeValid(st) && SystemTimeToFileTime(&st, &ft)) {
			uint64_t rawFT = (static_cast<uint64_t>(ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
			Encode8(buffer, rawFT);
		}
		else {
			Encode8(buffer, 0);
		}
	}

	static void EncodeStr(std::vector<uint8_t>& buffer, const std::wstring& wstr, size_t& uSize) {
		std::string str = WideChar2MultiByte(wstr);
		uSize = str.size();
		Encode2(buffer, static_cast<uint16_t>(uSize));
		if (uSize == 0) {
			return;
		}
		const char* pStr = str.c_str();
		const uint8_t* p = reinterpret_cast<const uint8_t*>(pStr);
		buffer.insert(buffer.end(), p, p + uSize);
	}

	static void EncodeBuffer(std::vector<uint8_t>& buffer, const std::wstring& data, size_t uSize) {
		switch (uSize) {
		case kCharacterNameLength: {
			std::string str = WideChar2MultiByte(data);
			size_t strLen = str.size();
			if (strLen > kCharacterNameLength) {
				str = str.substr(0, kCharacterNameLength);
			}
			else {
				str.append(kCharacterNameLength - strLen, '\0');
			}
			const char* pStr = str.c_str();
			const uint8_t* p = reinterpret_cast<const uint8_t*>(pStr);
			buffer.insert(buffer.end(), p, p + kCharacterNameLength);
			break;
		}
		case kFileTimeLength: {
			EncodeFT(buffer, data);
			break;
		}
		default:
			std::vector<uint8_t> tempBuffer;
			bool ok = Data2Buffer(data, tempBuffer);
			if (ok) {
				buffer.insert(buffer.end(), tempBuffer.begin(), tempBuffer.end());
			}
			else {
				std::vector<uint8_t> emptyBuffer(uSize);
				buffer.insert(buffer.end(), emptyBuffer.begin(), emptyBuffer.end());
			}
		}
	}

}

