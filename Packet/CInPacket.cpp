#include <map>
#include "CInPacket.h"

#include<intrin.h>
#pragma intrinsic(_ReturnAddress)

namespace {
	static std::map<void*, std::vector<PacketAction>> gActionsMap;

	static bool IsPayload(void* ecx) {
		InPacket* iPacket = static_cast<InPacket*>(ecx);
		return iPacket->m_nState == 0x02;
	}
}

namespace CInPacket {

	std::vector<PacketAction>& GetActions(void* key)
	{
		return gActionsMap[key];
	}

	void DeleteActions(void* key)
	{
		gActionsMap.erase(key);
	}

	uint8_t(__thiscall* Decode1)(void* ecx) = nullptr;
	uint8_t __fastcall Decode1_Hook(void* ecx) {
		auto& gActions = GetActions(ecx);
		gActions.push_back(PacketAction{ PacketActionType::Decode1,1,(uint32_t)_ReturnAddress() });
		return Decode1(ecx);
	}

	uint16_t(__thiscall* Decode2)(void* ecx) = nullptr;
	uint16_t __fastcall Decode2_Hook(void* ecx) {
		if (IsPayload(ecx)) {
			// CInPacket::AppendBuffer will call twice CInPacket::Decode2 as header in GMS95
			auto& gActions = GetActions(ecx);
			gActions.push_back(PacketAction{ PacketActionType::Decode2,2,(uint32_t)_ReturnAddress() });
		}
		return Decode2(ecx);
	}

	uint32_t(__thiscall* Decode4)(void* ecx) = nullptr;
	uint32_t __fastcall Decode4_Hook(void* ecx) {
		if (IsPayload(ecx)) {
			// CInPacket::AppendBuffer will call once CInPacket::Decode4 as header in KMS1029
			auto& gActions = GetActions(ecx);
			gActions.push_back(PacketAction{ PacketActionType::Decode4,4,(uint32_t)_ReturnAddress() });
		}
		return Decode4(ecx);
	}

	uint64_t(__thiscall* Decode8)(void* ecx) = nullptr;
	uint64_t __fastcall Decode8_Hook(void* ecx) {
		auto& gActions = GetActions(ecx);
		gActions.push_back(PacketAction{ PacketActionType::Decode8,8,(uint32_t)_ReturnAddress() });
		return Decode8(ecx);
	}

	char** (__thiscall* DecodeStr)(void* ecx, char** result) = nullptr;
	char** __fastcall DecodeStr_Hook(void* ecx, void* edx, char** result) {
		auto& gActions = GetActions(ecx);
		gActions.push_back(PacketAction{ PacketActionType::DecodeStr,0,(uint32_t)_ReturnAddress() });
		return DecodeStr(ecx, result);
	}

	void(__thiscall* DecodeBuffer)(void* ecx, uint8_t* p, size_t uSize) = nullptr;
	void __fastcall DecodeBuffer_Hook(void* ecx, void* edx, uint8_t* p, size_t uSize) {
		auto& gActions = GetActions(ecx);
		gActions.push_back(PacketAction{ PacketActionType::DecodeBuffer,uSize,(uint32_t)_ReturnAddress() });
		DecodeBuffer(ecx, p, uSize);
	}

}
