#include "pch.h"
#include "COutPacket.h"
#include "Router.h"

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
		gActionsMap[key] = actions;
	}

	void(__thiscall* Encode1)(OutPacket* oPacket, uint8_t n) = nullptr;
	void __fastcall Encode1_Hook(OutPacket* oPacket, void* edx, uint8_t n) {
		// COutPacket::COutPacket(opcodeIs1Byte)
		auto& gActions = GetActions(oPacket);
		gActions.push_back(PacketAction{ PacketActionType::Encode1,1,(uint32_t)_ReturnAddress() });
		return Encode1(oPacket, n);
	}

	void(__thiscall* Encode2)(OutPacket* oPacket, uint16_t n) = nullptr;
	void __fastcall Encode2_Hook(OutPacket* oPacket, void* edx, uint16_t n) {
		// COutPacket::COutPacket(opcode)
		auto& gActions = GetActions(oPacket);
		gActions.push_back(PacketAction{ PacketActionType::Encode2,2,(uint32_t)_ReturnAddress() });
		return Encode2(oPacket, n);
	}

	void(__thiscall* Encode4)(OutPacket* oPacket, uint32_t n) = nullptr;
	void __fastcall Encode4_Hook(OutPacket* oPacket, void* edx, uint32_t n) {
		if (oPacket->m_uOffset > 0) {
			auto& gActions = GetActions(oPacket);
			gActions.push_back(PacketAction{ PacketActionType::Encode4,4,(uint32_t)_ReturnAddress() });
		}
		return Encode4(oPacket, n);
	}

	void(__thiscall* Encode8)(OutPacket* oPacket, uint64_t n) = nullptr;
	void __fastcall Encode8_Hook(OutPacket* oPacket, void* edx, uint64_t n) {
		if (oPacket->m_uOffset > 0) {
			auto& gActions = GetActions(oPacket);
			gActions.push_back(PacketAction{ PacketActionType::Encode8,8,(uint32_t)_ReturnAddress() });
		}
		return Encode8(oPacket, n);
	}

	void(__thiscall* EncodeStr)(OutPacket* oPacket, char* s) = nullptr;
	void __fastcall EncodeStr_Hook(OutPacket* oPacket, void* edx, char* s) {
		if (oPacket->m_uOffset > 0) {
			auto& gActions = GetActions(oPacket);
			gActions.push_back(PacketAction{ PacketActionType::EncodeStr,0,(uint32_t)_ReturnAddress() });
		}
		return EncodeStr(oPacket, s);
	}

	void(__thiscall* EncodeBuffer)(OutPacket* oPacket, uint8_t* p, uint32_t uSize) = nullptr;
	void __fastcall EncodeBuffer_Hook(OutPacket* oPacket, void* edx, uint8_t* p, uint32_t uSize) {
		if (oPacket->m_uOffset > 0) {
			auto& gActions = GetActions(oPacket);
			gActions.push_back(PacketAction{ PacketActionType::EncodeBuffer,uSize,(uint32_t)_ReturnAddress() });
		}
		return EncodeBuffer(oPacket, p, uSize);
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
		return MakeBufferList(oPacket, l, uSeqBase, puSeqKey, bEnc, dwKey);
	}

}