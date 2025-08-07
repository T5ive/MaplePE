#pragma once
#include "IMainController.h"

class LoggingServer final : public UDPServer
{
public:
	LoggingServer(IMainController* mainControllerImpl);
	~LoggingServer();
	void HandleDatagram(const void* buf, size_t bufLen, const std::string& clientIP, uint16_t clientPort) override;
	std::wstring NotifyPacketInfo(const PacketLogModel& log);
	std::wstring BroadcastPacketInfo(const PacketLogModel& log);
private:
	std::map<int, ClientEndpoint> m_clientEndpointMap;
	IMainController* m_mainControllerImpl = nullptr;

	bool sendPacketInfo(const PacketLogModel& log, const ClientEndpoint& clientEndpoint);
};