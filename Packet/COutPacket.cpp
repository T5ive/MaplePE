#include "COutPacket.h"
#include "Router.h"

#include<intrin.h>
#pragma intrinsic(_ReturnAddress)

namespace {
	static std::vector<PacketAction> gActions;
}

namespace COutPacket {

	std::vector<PacketAction>& GetActions()
	{
		return gActions;
	}

	void SetActions(const std::vector<PacketAction>& actions)
	{
		gActions = actions;
	}

	void ClearActions()
	{
		gActions.clear();
	}

	void(__thiscall* Encode1)(void* ecx, uint8_t n) = nullptr;
	void __fastcall Encode1_Hook(void* ecx, void* edx, uint8_t n) {
		//DEBUGW(L"Encode1");
		gActions.push_back(PacketAction{ PacketActionType::Encode1,1,(uint32_t)_ReturnAddress() });
		Encode1(ecx, n);
	}

	void(__thiscall* Encode2)(void* ecx, uint16_t n) = nullptr;
	void __fastcall Encode2_Hook(void* ecx, void* edx, uint16_t n) {
		//DEBUGW(L"Encode2");
		gActions.push_back(PacketAction{ PacketActionType::Encode2,2,(uint32_t)_ReturnAddress() });
		Encode2(ecx, n);
	}

	void(__thiscall* Encode4)(void* ecx, uint32_t n) = nullptr;
	void __fastcall Encode4_Hook(void* ecx, void* edx, uint32_t n) {
		//DEBUGW(L"Encode4");
		gActions.push_back(PacketAction{ PacketActionType::Encode4,4,(uint32_t)_ReturnAddress() });
		Encode4(ecx, n);
	}

	void(__thiscall* Encode8)(void* ecx, uint64_t n) = nullptr;
	void __fastcall Encode8_Hook(void* ecx, void* edx, uint64_t n) {
		//DEBUGW(L"Encode8");
		gActions.push_back(PacketAction{ PacketActionType::Encode8,8,(uint32_t)_ReturnAddress() });
		Encode8(ecx, n);
	}

	void(__thiscall* EncodeStr)(void* ecx, char* s) = nullptr;
	void __fastcall EncodeStr_Hook(void* ecx, void* edx, char* s) {
		//DEBUGW(L"EncodeStr");
		gActions.push_back(PacketAction{ PacketActionType::EncodeStr,0,(uint32_t)_ReturnAddress() });
		EncodeStr(ecx, s);
	}

	void(__thiscall* EncodeBuffer)(void* ecx, uint8_t* p, uint32_t uSize) = nullptr;
	void __fastcall EncodeBuffer_Hook(void* ecx, void* edx, uint8_t* p, uint32_t uSize) {
		//DEBUGW(L"EncodeBuffer");
		gActions.push_back(PacketAction{ PacketActionType::EncodeBuffer,uSize,(uint32_t)_ReturnAddress() });
		EncodeBuffer(ecx, p, uSize);
	}

	void(__thiscall* MakeBufferList)(OutPacket* oPacket, void* l, uint16_t uSeqBase, uint32_t* puSeqKey, int bEnc, uint32_t dwKey) = nullptr;
	void __fastcall MakeBufferList_Hook(OutPacket* oPacket, void* edx, void* l, uint16_t uSeqBase, uint32_t* puSeqKey, int bEnc, uint32_t dwKey) {
		PacketInfo info{};
		info.PID = Router::kPID;
		info.Index = Router::gPacketIndex++;
		info.IsInPacket = false;
		info.Payload = std::vector<uint8_t>(oPacket->m_aSendBuff, oPacket->m_aSendBuff + oPacket->m_uOffset);
		info.Actions = GetActions();
		Router::SendPacketInfo(info);
		ClearActions();
		MakeBufferList(oPacket, l, uSeqBase, puSeqKey, bEnc, dwKey);
		//DEBUGW(L"MakeBufferList");
	}

}
