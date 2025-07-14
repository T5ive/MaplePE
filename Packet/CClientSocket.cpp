#include "CClientSocket.h"
#include "Router.h"

#include<intrin.h>
#pragma intrinsic(_ReturnAddress)


namespace CClientSocket {

	void(__thiscall* ProcessPacket)(void* ecx, InPacket* iPacket) = nullptr;
	void __fastcall ProcessPacket_Hook(void* ecx, void* edx, InPacket* iPacket) {
		if (Router::gClientSocketPtr == nullptr) {
			Router::gClientSocketPtr = ecx;
		}
		ProcessPacket(ecx, iPacket);
		//DEBUGW(L"ProcessPacket");
		PacketInfo info{};
		info.PID = Router::kPID;
		info.Index = Router::gPacketIndex++;
		info.IsInPacket = true;
		info.Payload = std::vector<uint8_t>(iPacket->m_aRecvBuff + Router::kHeaderLength, iPacket->m_aRecvBuff + iPacket->m_uLength);
		info.Actions = CInPacket::GetActions();
		Router::SendPacketInfo(info);
		CInPacket::ClearActions();
	}

	void(__thiscall* SendPacket)(void* ecx, OutPacket* oPacket) = nullptr;
	//void __fastcall SendPacket_Hook(void* ecx, void* edx, OutPacket* oPacket) {
	//	if (Router::gClientSocketPtr == nullptr) {
	//		Router::gClientSocketPtr = ecx;
	//	}
	//	PacketInfo info{};
	//	info.PID = Router::kPID;
	//	info.Index = Router::gPacketIndex++;
	//	info.IsInPacket = false;
	//	info.Payload = std::vector<uint8_t>(oPacket->m_aSendBuff, oPacket->m_aSendBuff + oPacket->m_uOffset);
	//	info.Actions = COutPacket::GetActions();
	//	Router::SendPacketInfo(info);
	//	COutPacket::ClearActions();
	//	SendPacket(ecx, oPacket);
	//}

}
