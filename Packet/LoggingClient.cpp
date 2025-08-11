#include "pch.h"
#include "LoggingClient.h"
#include "Router.h"

LoggingClient::LoggingClient(const std::wstring& serverIP, uint16_t serverPort) :UDPClient(serverIP, serverPort)
{
}

LoggingClient::~LoggingClient()
{
}

void LoggingClient::SendPacketInfo(PacketInfo& info)
{
	std::vector<uint8_t> buffer = PacketFormat::Serialize(info);
	bool ok = this->SendBuffer(buffer.data(), buffer.size());
	if (!ok) {
		DEBUGW(L"Failed to send packet info");
		return;
	}
}

void LoggingClient::HandleDatagram(const void* buf, size_t bufLen, const std::string& clientIP, uint16_t clientPort)
{
	const uint8_t* data = static_cast<const uint8_t*>(buf);
	std::vector<uint8_t> buffer(data, data + bufLen);
	PacketInfo info = PacketFormat::Deserialize(buffer);
	if (info.IsInPacket) {
		InPacket iPacket{};
		iPacket.m_nState = 2;
		iPacket.m_uDataLen = static_cast<uint16_t>(info.Payload.size());
		iPacket.m_uLength = Router::kHeaderLength + iPacket.m_uDataLen;
		iPacket.m_aRecvBuff = new uint8_t[iPacket.m_uLength];
		iPacket.m_uOffset = Router::kHeaderLength;
		memcpy(iPacket.m_aRecvBuff + Router::kHeaderLength, info.Payload.data(), info.Payload.size());
		Router::ProcessPacket(&iPacket);
		delete[] iPacket.m_aRecvBuff;
	}
	else {
		OutPacket oPacket{};
		oPacket.m_aSendBuff = new uint8_t[info.Payload.size()];
		oPacket.m_uOffset = info.Payload.size();
		memcpy(oPacket.m_aSendBuff, info.Payload.data(), info.Payload.size());
		COutPacket::SetActions(&oPacket, info.Actions);
		Router::SendPacket(&oPacket);
		delete[] oPacket.m_aSendBuff;
	}
}