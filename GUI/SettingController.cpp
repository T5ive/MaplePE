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
		std::to_wstring(s.IsTypeHeader1Byte),
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
	size_t index = 0;
	Setting s{
		propValues[index], // GameProcessName
		propValues[++index], // PacketDllName
		propValues[++index], // LoggingServerIP
		WSTR2ULONGPTR(propValues[++index]),  // LoggingServerPort
		WSTR2BOOL(propValues[++index]),	   // IsTypeHeader1Byte
		WSTR2ULONGPTR(propValues[++index]),  // CInPacketDecode1Addr
		WSTR2ULONGPTR(propValues[++index]),  // CInPacketDecode2Addr
		WSTR2ULONGPTR(propValues[++index]),  // CInPacketDecode4Addr
		WSTR2ULONGPTR(propValues[++index]),  // CInPacketDecode8Addr
		WSTR2ULONGPTR(propValues[++index]),  // CInPacketDecodeStrAddr
		WSTR2ULONGPTR(propValues[++index]), // CInPacketDecodeBufferAddr
		WSTR2ULONGPTR(propValues[++index]), // COutPacketEncode1Addr
		WSTR2ULONGPTR(propValues[++index]), // COutPacketEncode2Addr
		WSTR2ULONGPTR(propValues[++index]), // COutPacketEncode4Addr
		WSTR2ULONGPTR(propValues[++index]), // COutPacketEncode8Addr
		WSTR2ULONGPTR(propValues[++index]), // COutPacketEncodeStrAddr
		WSTR2ULONGPTR(propValues[++index]), // COutPacketEncodeBufferAddr
		WSTR2ULONGPTR(propValues[++index]), // COutPacketMakeBufferListAddr
		WSTR2ULONGPTR(propValues[++index]), // CClientSocketProcessPacketAddr
		WSTR2ULONGPTR(propValues[++index]), // CClientSocketSendPacketAddr
		propValues[++index], // CInPacketDecode1GenCode
		propValues[++index], // CInPacketDecode2GenCode
		propValues[++index], // CInPacketDecode4GenCode
		propValues[++index], // CInPacketDecode8GenCode
		propValues[++index], // CInPacketDecodeStrGenCode
		propValues[++index], // CInPacketDecodeBufferGenCode
		propValues[++index], // COutPacketEncode1GenCode
		propValues[++index], // COutPacketEncode2GenCode
		propValues[++index], // COutPacketEncode4GenCode
		propValues[++index], // COutPacketEncode8GenCode
		propValues[++index], // COutPacketEncodeStrGenCode
		propValues[++index], // COutPacketEncodeBufferGenCode
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
	const Setting s = Serialize(propValues);
	return m_mainControllerImpl->SetSetting(s);
}

const std::vector<std::wstring> SettingController::ResetSetting()
{
	Setting s{};
	return Deserialize(s);
}
