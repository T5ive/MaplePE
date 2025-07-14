#include "SettingView.h"
#include "SettingController.h"


SettingController::SettingController(IMainController* mainControllerImpl, SettingView* view)
{
	m_mainControllerImpl = mainControllerImpl;
	m_settingView = view;
}

SettingController::~SettingController()
{

}

const std::vector<std::wstring> SettingController::Deserialize(const Setting& s)
{
	const std::vector<std::wstring> settings{
		s.GameProcessName,
		s.PacketDllName,
		s.LoggingServerIP,
		std::to_wstring(s.LoggingServerPort),
		PacketScript::Int2Hex(s.CInPacketDecode1Addr),
		PacketScript::Int2Hex(s.CInPacketDecode2Addr),
		PacketScript::Int2Hex(s.CInPacketDecode4Addr),
		PacketScript::Int2Hex(s.CInPacketDecode8Addr),
		PacketScript::Int2Hex(s.CInPacketDecodeStrAddr),
		PacketScript::Int2Hex(s.CInPacketDecodeBufferAddr),
		PacketScript::Int2Hex(s.COutPacketEncode1Addr),
		PacketScript::Int2Hex(s.COutPacketEncode2Addr),
		PacketScript::Int2Hex(s.COutPacketEncode4Addr),
		PacketScript::Int2Hex(s.COutPacketEncode8Addr),
		PacketScript::Int2Hex(s.COutPacketEncodeStrAddr),
		PacketScript::Int2Hex(s.COutPacketEncodeBufferAddr),
		PacketScript::Int2Hex(s.COutPacketMakeBufferListAddr),
		PacketScript::Int2Hex(s.CClientSocketProcessPacketAddr),
		PacketScript::Int2Hex(s.CClientSocketSendPacketAddr),
		s.CInPacketDecode1GenCode,
		s.CInPacketDecode2GenCode,
		s.CInPacketDecode4GenCode,
		s.CInPacketDecode8GenCode,
		s.CInPacketDecodeStrGenCode,
		s.CInPacketDecodeBufferGenCode,
		s.COutPacketEncode1GenCode,
		s.COutPacketEncode2GenCode,
		s.COutPacketEncode4GenCode,
		s.COutPacketEncode8GenCode,
		s.COutPacketEncodeStrGenCode,
		s.COutPacketEncodeBufferGenCode,
	};
	return settings;
}

const Setting SettingController::Serialize(const std::vector<std::wstring>& propValues)
{
	Setting s{
		propValues[0], // GameProcessName
		propValues[1], // PacketDllName
		propValues[2], // LoggingServerIP
		WSTR2ULONGPTR(propValues[3]),  // LoggingServerPort
		WSTR2ULONGPTR(propValues[4]),  // CInPacketDecode1Addr
		WSTR2ULONGPTR(propValues[5]),  // CInPacketDecode2Addr
		WSTR2ULONGPTR(propValues[6]),  // CInPacketDecode4Addr
		WSTR2ULONGPTR(propValues[7]),  // CInPacketDecode8Addr
		WSTR2ULONGPTR(propValues[8]),  // CInPacketDecodeStrAddr
		WSTR2ULONGPTR(propValues[9]),  // CInPacketDecodeBufferAddr
		WSTR2ULONGPTR(propValues[10]), // COutPacketEncode1Addr
		WSTR2ULONGPTR(propValues[11]), // COutPacketEncode2Addr
		WSTR2ULONGPTR(propValues[12]), // COutPacketEncode4Addr
		WSTR2ULONGPTR(propValues[13]), // COutPacketEncode8Addr
		WSTR2ULONGPTR(propValues[14]), // COutPacketEncodeStrAddr
		WSTR2ULONGPTR(propValues[15]), // COutPacketEncodeBufferAddr
		WSTR2ULONGPTR(propValues[16]), // COutPacketMakeBufferListAddr
		WSTR2ULONGPTR(propValues[17]), // CClientSocketProcessPacketAddr
		WSTR2ULONGPTR(propValues[18]), // CClientSocketSendPacketAddr
		propValues[19], // CInPacketDecode1GenCode
		propValues[20], // CInPacketDecode2GenCode
		propValues[21], // CInPacketDecode4GenCode
		propValues[22], // CInPacketDecode8GenCode
		propValues[23], // CInPacketDecodeStrGenCode
		propValues[24], // CInPacketDecodeBufferGenCode
		propValues[25], // COutPacketEncode1GenCode
		propValues[26], // COutPacketEncode2GenCode
		propValues[27], // COutPacketEncode4GenCode
		propValues[28], // COutPacketEncode8GenCode
		propValues[29], // COutPacketEncodeStrGenCode
		propValues[30], // COutPacketEncodeBufferGenCode
	};
	return s;
}

const std::vector<std::wstring> SettingController::GetSettings()
{
	const Setting s = m_mainControllerImpl->GetSetting();
	return Deserialize(s);
}

bool SettingController::SetSetting(const std::vector<std::wstring> propValues)
{
	if (propValues.size() != 31) {
		return false;
	}
	const Setting s = Serialize(propValues);
	return m_mainControllerImpl->SetSetting(s);
}

const std::vector<std::wstring> SettingController::ResetSetting()
{
	Setting s{};
	return Deserialize(s);
}
