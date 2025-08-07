#pragma once
#include "CInPacket.h"
#include "COutPacket.h"

class LoggingClient final : public UDPClient {
public:
	LoggingClient(const std::wstring& serverIP, const uint16_t serverPort);
	~LoggingClient();
	void SendPacketInfo(PacketInfo& info);
protected:
	void HandleDatagram(const void* buf, size_t bufLen, const std::string& clientIP, uint16_t clientPort) override;
private:
	static LoggingClient* s_instance;
};