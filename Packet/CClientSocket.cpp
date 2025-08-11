#include "pch.h"
#include "CClientSocket.h"
#include "Router.h"

namespace CClientSocket {

	void(__thiscall* ProcessPacket)(void* ecx, void* packet) = nullptr;
	void __fastcall ProcessPacket_Hook(void* ecx, void* edx, void* packet) {
		if (Router::gClientSocketPtr == nullptr) {
			Router::gClientSocketPtr = ecx;
		}
		ProcessPacket(ecx, packet);
		auto actions = CInPacket::GetActions(packet);
		if (actions != nullptr && !actions->empty()) {
			InPacket* iPacket = static_cast<InPacket*>(packet);
			PacketInfo info{};
			info.PID = Router::kPID;
			info.Index = Router::gPacketIndex++;
			info.IsInPacket = true;
			info.Payload = std::vector<uint8_t>(iPacket->m_aRecvBuff + Router::kHeaderLength, iPacket->m_aRecvBuff + iPacket->m_uLength);
			info.Actions = *actions;
			Router::SendPacketInfo(info);
		}
		CInPacket::DeleteActions(packet);
	}

	void(__thiscall* SendPacket)(void* ecx, void* packet) = nullptr;

}