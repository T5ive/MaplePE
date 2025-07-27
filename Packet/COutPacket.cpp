#include <map>
#include "COutPacket.h"
#include "Router.h"


#include<intrin.h>
#pragma intrinsic(_ReturnAddress)

namespace {
	static std::map<void*, std::vector<PacketAction>> gActionsMap;
}

namespace COutPacket {

	std::vector<PacketAction>& GetActions(void* key) {
		return gActionsMap[key];
	}

	void DeleteActions(void* key)
	{
		gActionsMap.erase(key);
	}

	void SetActions(void* key, const std::vector<PacketAction>& actions)
	{
		auto& gActions = GetActions(key);
		gActions = actions;
	}

	void(__thiscall* Encode1)(void* ecx, uint8_t n) = nullptr;
	void __fastcall Encode1_Hook(void* ecx, void* edx, uint8_t n) {
		auto& gActions = GetActions(ecx);
		gActions.push_back(PacketAction{ PacketActionType::Encode1,1,(uint32_t)_ReturnAddress() });
		Encode1(ecx, n);
	}

	void(__thiscall* Encode2)(void* ecx, uint16_t n) = nullptr;
	void __fastcall Encode2_Hook(void* ecx, void* edx, uint16_t n) {
		auto& gActions = GetActions(ecx);
		gActions.push_back(PacketAction{ PacketActionType::Encode2,2,(uint32_t)_ReturnAddress() });
		Encode2(ecx, n);
	}

	void(__thiscall* Encode4)(void* ecx, uint32_t n) = nullptr;
	void __fastcall Encode4_Hook(void* ecx, void* edx, uint32_t n) {
		auto& gActions = GetActions(ecx);
		gActions.push_back(PacketAction{ PacketActionType::Encode4,4,(uint32_t)_ReturnAddress() });
		Encode4(ecx, n);
	}

	void(__thiscall* Encode8)(void* ecx, uint64_t n) = nullptr;
	void __fastcall Encode8_Hook(void* ecx, void* edx, uint64_t n) {
		auto& gActions = GetActions(ecx);
		gActions.push_back(PacketAction{ PacketActionType::Encode8,8,(uint32_t)_ReturnAddress() });
		Encode8(ecx, n);
	}

	void(__thiscall* EncodeStr)(void* ecx, char* s) = nullptr;
	void __fastcall EncodeStr_Hook(void* ecx, void* edx, char* s) {
		auto& gActions = GetActions(ecx);
		gActions.push_back(PacketAction{ PacketActionType::EncodeStr,0,(uint32_t)_ReturnAddress() });
		EncodeStr(ecx, s);
	}

	void(__thiscall* EncodeBuffer)(void* ecx, uint8_t* p, uint32_t uSize) = nullptr;
	void __fastcall EncodeBuffer_Hook(void* ecx, void* edx, uint8_t* p, uint32_t uSize) {
		auto& gActions = GetActions(ecx);
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
		void* key = (void*)oPacket;
		info.Actions = GetActions(key);
		Router::SendPacketInfo(info);
		DeleteActions(key);
		MakeBufferList(oPacket, l, uSeqBase, puSeqKey, bEnc, dwKey);
	}

}
