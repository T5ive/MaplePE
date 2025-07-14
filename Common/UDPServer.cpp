#include "UDPServer.h"
#include "WSAData.h"

#define RECV_BUF_LEN 8192

UDPServer::UDPServer()
{
	m_name = L"UDPServer";
}

UDPServer::~UDPServer()
{
	int iResult = closesocket(m_recvSocket);
	if (iResult == SOCKET_ERROR) {
		std::wstring msg = L"closesocket failed with error " + std::to_wstring(WSAGetLastError());
		MessageBox(nullptr, msg.c_str(), m_name.c_str(), MB_OK);
		m_recvSocket = INVALID_SOCKET;
	}
	if (m_thread.joinable()) {
		m_thread.join();
	}
	WSACleanup();
}

bool UDPServer::Bind(std::wstring sIP, uint16_t wPort)
{
	if (m_isBound) {
		return false;
	}
	if (!InitWSAData()) {
		return false;
	}
	// Resolve the local address and port to be used by the server
	std::wstring portStr = std::to_wstring(wPort);
	ADDRINFOW* result = NULL;
	ADDRINFOW hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;
	hints.ai_flags = AI_PASSIVE;
	int iResult = GetAddrInfoW(sIP.c_str(), portStr.c_str(), &hints, &result);
	if (iResult != 0) {
		std::wstring err = L"GetAddrInfoW failed: " + std::to_wstring(iResult);
		MessageBox(nullptr, err.c_str(), m_name.c_str(), MB_OK);
		return false;
	}
	// Create a SOCKET for the server to receive datagrams
	m_recvSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (m_recvSocket == INVALID_SOCKET) {
		std::wstring err = L"Error at socket(): " + std::to_wstring(WSAGetLastError());
		MessageBox(nullptr, err.c_str(), m_name.c_str(), MB_OK);
		FreeAddrInfoW(result);
		return false;
	}
	// Setup the UDP listening socket
	iResult = bind(m_recvSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		std::wstring err = L"bind failed with error: " + std::to_wstring(WSAGetLastError());
		MessageBox(nullptr, err.c_str(), m_name.c_str(), MB_OK);
		FreeAddrInfoW(result);
		closesocket(m_recvSocket);
		return false;
	}
	FreeAddrInfoW(result);
	m_isBound = true;
	return true;
}

void UDPServer::Run()
{
	m_thread = std::thread([this]() {
		StartListen(m_name, m_recvSocket, [this](const char* data, int len, const std::string& ip, uint16_t port) {
			this->HandleDatagram(data, len, ip, port);
			});
		});
}

bool UDPServer::Notify(const void* buf, size_t bufLen, const std::string& clientIP, uint16_t clientPort)
{
	sockaddr_in clientAddr = {};
	clientAddr.sin_family = AF_INET;
	if (inet_pton(AF_INET, clientIP.c_str(), &clientAddr.sin_addr) != 1) {
		std::wstring msg = L"Faild to set client ip";
		MessageBox(nullptr, msg.c_str(), m_name.c_str(), MB_OK);
		return false;
	};
	clientAddr.sin_port = htons(clientPort);
	int iResult = sendto(m_recvSocket, static_cast<const char*>(buf), bufLen, 0, (SOCKADDR*)&clientAddr, sizeof(clientAddr));
	if (iResult == SOCKET_ERROR) {
		int err = WSAGetLastError();
		std::wstring msg = L"Notify failed with error: " + std::to_wstring(err);
		MessageBox(nullptr, msg.c_str(), m_name.c_str(), MB_OK);
		return false;
	}
	return true;
}



