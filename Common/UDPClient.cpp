#include "UDPClient.h"
#include "WSAData.h"
#include "Tool.h"

UDPClient::UDPClient(std::wstring serverIP, uint16_t serverPort)
{
	m_name = L"UDPClient";
	if (!InitWSAData()) {
		DEBUGW(L"WSAStartup failed");
		return;
	}
	// Resolve the local address and port to be used by the client
	std::wstring portStr = std::to_wstring(serverPort);
	ADDRINFOW* result = nullptr;
	ADDRINFOW hints = {};
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_protocol = IPPROTO_UDP;

	int iResult = GetAddrInfoW(serverIP.c_str(), portStr.c_str(), &hints, &result);
	if (iResult != 0) {
		DEBUGW(L"GetAddrInfoW failed: " + std::to_wstring(iResult));
		return;
	}
	// Create a socket for sending data
	m_sendSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (m_sendSocket == INVALID_SOCKET) {
		std::wstring err = L"error at socket(): " + std::to_wstring(WSAGetLastError());
		DEBUGW(err);
		FreeAddrInfoW(result);
		return;
	}
	memcpy(&m_serverAddr, result->ai_addr, result->ai_addrlen);
	// Bind to a random local port for receiving server message
	sockaddr_in localAddr = {};
	localAddr.sin_family = AF_INET;
	localAddr.sin_addr.s_addr = INADDR_ANY;
	localAddr.sin_port = 0;
	if (bind(m_sendSocket, (sockaddr*)&localAddr, sizeof(localAddr)) == SOCKET_ERROR) {
		std::wstring err = L"bind() failed: " + std::to_wstring(WSAGetLastError());
		DEBUGW(err);
		FreeAddrInfoW(result);
		closesocket(m_sendSocket);
		return;
	}
	FreeAddrInfoW(result);
}

UDPClient::~UDPClient()
{
	int iResult = closesocket(m_sendSocket);
	if (iResult == SOCKET_ERROR) {
		std::wstring err = L"closesocket failed with error: " + std::to_wstring(WSAGetLastError());
		DEBUGW(err);
		m_sendSocket = INVALID_SOCKET;
	}
	if (m_thread.joinable()) {
		m_thread.join();
	}
	WSACleanup();
}

bool UDPClient::SendBuffer(const void* buf, size_t bufLen)
{
	int iResult = sendto(m_sendSocket, static_cast<const char*>(buf), bufLen, 0, (SOCKADDR*)&m_serverAddr, sizeof(m_serverAddr));
	if (iResult == SOCKET_ERROR) {
		int err = WSAGetLastError();
		switch (err) {
		case WSAENOTSOCK:
			DEBUGW(L"sendto() failed: socket was already closed");
			break;
		case WSAEINTR:
			DEBUGW(L"sendto() was interrupted, possibly due to thread shutdown");
			break;
		case WSAEMSGSIZE:
			DEBUGW(L"Packet too large for UDP MTU");
		default:
			DEBUGW(L"Unknown err" + std::to_wstring(err));
		}
		return false;
	}
	return true;
}

void UDPClient::OnNotify()
{
	m_thread = std::thread([this]() {
		StartListen(m_name, m_sendSocket, [this](const char* data, int len, const std::string& ip, uint16_t port) {
			this->HandleDatagram(data, len, ip, port);
			});
		});
}