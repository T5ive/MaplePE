#include "pch.h"
#include "CInPacket.h"

namespace {

	static std::set<uint16_t> gFilterOpcodeSet;

	static std::map<void*, std::vector<PacketAction>> gActionsMap;

	static bool IsPayload(void* ecx) {
		InPacket* iPacket = static_cast<InPacket*>(ecx);
		return iPacket->m_nState == 2;
	}
}

namespace CInPacket {

	void InitFilterOpcodeSet(std::wstring& opcodes)
	{
		std::wstringstream ss(opcodes);
		std::wstring token;

		while (std::getline(ss, token, L',')) {
			try {
				uint16_t opcode = static_cast<uint16_t>(std::stoul(token, nullptr, 0));
				gFilterOpcodeSet.insert(opcode);
			}
			catch (const std::exception&) {
				DEBUGW(L"Unknown filter opcodes")
			}
		}
	}

	bool IsFilterOpcode(uint16_t opcode)
	{
		return gFilterOpcodeSet.find(opcode) != gFilterOpcodeSet.end();
	}

	std::vector<PacketAction>* GetActions(void* key)
	{
		auto it = gActionsMap.find(key);
		if (it == gActionsMap.end()) {
			return nullptr;
		}
		return &it->second;
	}

	void DeleteActions(void* key)
	{
		gActionsMap.erase(key);
	}

	uint8_t(__thiscall* Decode1)(void* ecx) = nullptr;
	uint8_t __fastcall Decode1_Hook(void* ecx) {
		uint8_t result = Decode1(ecx);
		if (IsPayload(ecx)) {
			auto action = PacketAction{ PacketActionType::Decode1,1,(uint32_t)_ReturnAddress() };
			auto actions = GetActions(ecx);
			if (actions == nullptr) {
				if (IsFilterOpcode(result)) {
					gActionsMap[ecx] = std::vector<PacketAction>{};
				}
				else {
					gActionsMap[ecx] = std::vector<PacketAction>{ action };
				}
			}
			else if (!actions->empty()) {
				actions->push_back(action);
			}
		}
		return result;
	}

	uint16_t(__thiscall* Decode2)(void* ecx) = nullptr;
	uint16_t __fastcall Decode2_Hook(void* ecx) {
		uint16_t result = Decode2(ecx);
		if (IsPayload(ecx)) {
			// CInPacket::AppendBuffer will call twice CInPacket::Decode2 as header in GMS95
			auto action = PacketAction{ PacketActionType::Decode2,2,(uint32_t)_ReturnAddress() };
			auto actions = GetActions(ecx);
			if (actions == nullptr) {
				if (IsFilterOpcode(result)) {
					gActionsMap[ecx] = std::vector<PacketAction>{};
				}
				else {
					gActionsMap[ecx] = std::vector<PacketAction>{ action };
				}
			}
			else if (!actions->empty()) {
				actions->push_back(PacketAction{ action });
			}
		}
		return result;
	}

	uint32_t(__thiscall* Decode4)(void* ecx) = nullptr;
	uint32_t __fastcall Decode4_Hook(void* ecx) {
		if (IsPayload(ecx)) {
			// CInPacket::AppendBuffer will call once CInPacket::Decode4 as header in KMS1029
			auto actions = GetActions(ecx);
			if (actions != nullptr && !actions->empty()) {
				actions->push_back(PacketAction{ PacketActionType::Decode4,4,(uint32_t)_ReturnAddress() });
			}
		}
		return Decode4(ecx);
	}

	uint64_t(__thiscall* Decode8)(void* ecx) = nullptr;
	uint64_t __fastcall Decode8_Hook(void* ecx) {
		if (IsPayload(ecx)) {
			auto actions = GetActions(ecx);
			if (actions != nullptr && !actions->empty()) {
				actions->push_back(PacketAction{ PacketActionType::Decode8,8,(uint32_t)_ReturnAddress() });
			}
		}
		return Decode8(ecx);
	}

	char** (__thiscall* DecodeStr)(void* iPacket, char** result) = nullptr;
	char** __fastcall DecodeStr_Hook(void* ecx, void* edx, char** result) {
		if (IsPayload(ecx)) {
			auto actions = GetActions(ecx);
			if (actions != nullptr && !actions->empty()) {
				actions->push_back(PacketAction{ PacketActionType::DecodeStr,0,(uint32_t)_ReturnAddress() });
			}
		}
		return DecodeStr(ecx, result);
	}

	void(__thiscall* DecodeBuffer)(void* ecx, uint8_t* p, size_t uSize) = nullptr;
	void __fastcall DecodeBuffer_Hook(void* ecx, void* edx, uint8_t* p, size_t uSize) {
		if (IsPayload(ecx)) {
			auto actions = GetActions(ecx);
			if (actions != nullptr && !actions->empty()) {
				actions->push_back(PacketAction{ PacketActionType::DecodeBuffer,uSize,(uint32_t)_ReturnAddress() });
			}
		}
		return DecodeBuffer(ecx, p, uSize);
	}

}