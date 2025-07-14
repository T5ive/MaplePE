#include "UDPServer.h"
#include "LoggingServer.h"

#ifdef _DEBUG
#pragma comment(lib, "CommonD.lib")
#else
#pragma comment(lib, "Common.lib")
#endif

LoggingServer::LoggingServer(IMainController* mainControllerImpl) :UDPServer()
{
	m_mainControllerImpl = mainControllerImpl;
	const Setting s = mainControllerImpl->GetSetting();
	this->Bind(s.LoggingServerIP, static_cast<uint16_t>(s.LoggingServerPort));
}

LoggingServer::~LoggingServer()
{
}

void LoggingServer::HandleDatagram(const void* buf, size_t bufLen, const std::string& clientIP, uint16_t clientPort)
{
	const uint8_t* data = static_cast<const uint8_t*>(buf);
	std::vector<uint8_t> buffer(data, data + bufLen);
	PacketInfo info = PacketFormat::Deserialize(buffer);
	PacketLogModel log(info);
	m_clientEndpointMap[log.GetPID()] = ClientEndpoint{
		clientIP,
		clientPort,
	};
	m_mainControllerImpl->OnPacketLogModel(log);

}

std::wstring LoggingServer::NotifyPacketInfo(const PacketLogModel& log)
{
	if (log.IsEmpty()) {
		return L"Invaild PID";
	}
	const int pid = log.GetPID();
	auto it = m_clientEndpointMap.find(pid);
	if (it == m_clientEndpointMap.end()) {
		return L"No client found for this PID";
	}
	const ClientEndpoint& clientEndpoint = it->second;
	bool ok = this->sendPacketInfo(log, clientEndpoint);
	if (!ok) {
		m_clientEndpointMap.erase(pid);
		return L"Failed to send packet info";
	}
	return std::wstring();
}

std::wstring LoggingServer::BroadcastPacketInfo(const PacketLogModel& log)
{
	if (!log.IsEmpty()) {
		return L"PID must be 0 if broadcasting";
	}
	if (m_clientEndpointMap.size() == 0) {
		return L"No active clients";
	}
	for (auto it = m_clientEndpointMap.begin(); it != m_clientEndpointMap.end(); ) {
		const ClientEndpoint& clientEndpoint = it->second;
		bool ok = this->sendPacketInfo(log, clientEndpoint);
		if (!ok) {
			it = m_clientEndpointMap.erase(it);
		}
		else {
			++it;
		}
	}
	return std::wstring();
}

bool LoggingServer::sendPacketInfo(const PacketLogModel& log, const ClientEndpoint& clientEndpoint)
{
	PacketInfo info{};
	info.IsInPacket = log.IsInPacket();
	info.Actions = log.GetActions();
	bool ok = PacketScript::Data2Buffer(log.GetData(), info.Payload);
	if (!ok) {
		return false;
	}
	std::vector<uint8_t> buffer = PacketFormat::Serialize(info);
	return this->Notify(buffer.data(), buffer.size(), clientEndpoint.IP, clientEndpoint.Port);
}




