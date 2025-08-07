#include "pch.h"
#include "CClientSocket.h"
#include "Router.h"

namespace CClientSocket {

	void(__thiscall* ProcessPacket)(void* ecx, InPacket* iPacket) = nullptr;
	void __fastcall ProcessPacket_Hook(void* ecx, void* edx, InPacket* iPacket) {
		if (Router::gClientSocketPtr == nullptr) {
			Router::gClientSocketPtr = ecx;
		}
		ProcessPacket(ecx, iPacket);
		PacketInfo info{};
		info.PID = Router::kPID;
		info.Index = Router::gPacketIndex++;
		info.IsInPacket = true;
		info.Payload = std::vector<uint8_t>(iPacket->m_aRecvBuff + Router::kHeaderLength, iPacket->m_aRecvBuff + iPacket->m_uLength);
		void* key = (void*)iPacket;
		info.Actions = CInPacket::GetActions(key);
		Router::SendPacketInfo(info);
		CInPacket::DeleteActions(key);
	}

	void(__thiscall* SendPacket)(void* ecx, OutPacket* oPacket) = nullptr;

}
