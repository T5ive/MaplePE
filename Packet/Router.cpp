#include "Router.h"
#include "CClientSocket.h"
#include "LoggingClient.h"

namespace {
	static LoggingClient* gLoggingClient = nullptr;
}

namespace Router {

	const uint32_t kPID = GetCurrentProcessId();
	uint32_t gPacketIndex = 0;
	void* gClientSocketPtr = nullptr;

	void Init(const std::wstring& serverIP, const uint16_t serverPort) {
		if (gLoggingClient != nullptr) {
			DEBUGW(L"LoggingClient already initialized");
			return;
		}
		gLoggingClient = new LoggingClient(serverIP, serverPort);
		gLoggingClient->OnNotify();
	}

	void Free() {
		delete gLoggingClient;
		gLoggingClient = nullptr;
	}

	void SendPacketInfo(PacketInfo& info) {
		if (gLoggingClient == nullptr) {
			return;
		}
		gLoggingClient->SendPacketInfo(info);
	}

	void ProcessPacket(InPacket* iPacket) {
		if (gClientSocketPtr == nullptr) {
			DEBUGW(L"ClientSocket has not been specified");
			return;
		}
		CClientSocket::ProcessPacket_Hook(gClientSocketPtr, nullptr, iPacket);
	}

	void SendPacket(OutPacket* oPacket) {
		if (gClientSocketPtr == nullptr) {
			DEBUGW(L"ClientSocket has not been specified");
			return;
		}
		CClientSocket::SendPacket(gClientSocketPtr, oPacket);
	}
}