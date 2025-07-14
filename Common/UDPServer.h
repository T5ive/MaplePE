#pragma once
#include <winsock2.h>
#include <string>
#include <thread>

class UDPServer {
public:
	UDPServer();
	~UDPServer();
	bool Bind(std::wstring sIP, uint16_t wPort);
	void Run();
protected:
	bool Notify(const void* buf, size_t bufLen, const std::string& clientIP, uint16_t clientPort);
	virtual void HandleDatagram(const void* buf, size_t bufLen, const std::string& clientIP, uint16_t clientPort) = 0;
private:
	std::wstring m_name;
	bool m_isBound = false;
	SOCKET m_recvSocket = INVALID_SOCKET;
	std::thread m_thread;
};