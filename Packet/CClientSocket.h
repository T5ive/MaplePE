#pragma once
#include "CInPacket.h"
#include "COutPacket.h"

namespace CClientSocket {

	// GMS95 void __thiscall CClientSocket::ProcessPacket(CClientSocket *this, CInPacket *iPacket)
	extern void(__thiscall* ProcessPacket)(void* ecx, InPacket* iPacket);
	void __fastcall ProcessPacket_Hook(void* ecx, void* edx, InPacket* iPacket);

	// GMS95 void __fastcall CClientSocket::SendPacket(CClientSocket *this, int a2, COutPacket *oPacket)
	extern void(__thiscall* SendPacket)(void* ecx, OutPacket* oPacket);
	// void __fastcall SendPacket_Hook(void* ecx, void* edx, OutPacket* oPacket);

}
