#pragma once
#include <winsock2.h>
#include <string>
#include <thread>

class UDPClient {
public:
	UDPClient(std::wstring serverIP, uint16_t serverPort);
	~UDPClient();
	bool SendBuffer(const void* buf, size_t bufLen);
	void OnNotify();
protected:
	virtual void HandleDatagram(const void* buf, size_t bufLen, const std::string& clientIP, uint16_t clientPort) = 0;
private:
	std::wstring m_name;
	SOCKET m_sendSocket = INVALID_SOCKET;
	sockaddr_in m_serverAddr{};
	std::thread m_thread;
};