#pragma once
#include "CInPacket.h"
#include "COutPacket.h"

namespace Router {
	constexpr uint8_t kHeaderLength = 4;
	extern const uint32_t kPID;
	extern uint32_t gPacketIndex;
	extern void* gClientSocketPtr;

	void Init(const std::wstring& serverIP, const uint16_t serverPort);
	void Free();
	void SendPacketInfo(PacketInfo& info);
	void ProcessPacket(InPacket* iPacket);
	void SendPacket(OutPacket* oPacket);
}