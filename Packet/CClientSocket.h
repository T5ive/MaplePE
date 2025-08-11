#pragma once
#include "CInPacket.h"
#include "COutPacket.h"

namespace CClientSocket {

	// GMS95 void __thiscall CClientSocket::ProcessPacket(CClientSocket *this, CInPacket *iPacket)
	extern void(__thiscall* ProcessPacket)(void* ecx, void* packet);
	void __fastcall ProcessPacket_Hook(void* ecx, void* edx, void* packet);

	// GMS95 void __fastcall CClientSocket::SendPacket(CClientSocket *this, int a2, COutPacket *oPacket)
	extern void(__thiscall* SendPacket)(void* ecx, void* packet);
	// void __fastcall SendPacket_Hook(void* ecx, void* edx, void* packet);

}