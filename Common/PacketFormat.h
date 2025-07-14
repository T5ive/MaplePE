#pragma once
#include "PacketScript.h"

enum PacketActionType {
	Decode1,
	Decode2,
	Decode4,
	Decode8,
	DecodeStr,
	DecodeBuffer,
	Encode1,
	Encode2,
	Encode4,
	Encode8,
	EncodeStr,
	EncodeBuffer,
};


struct PacketAction {
	PacketActionType Type;
	uint32_t Size;
	uint32_t RetAddr;
};

struct PacketInfo {
	uint32_t PID;
	uint32_t Index;
	bool IsInPacket;
	std::vector<uint8_t> Payload;
	std::vector<PacketAction> Actions;
};

namespace PacketFormat {

	static std::vector<uint8_t> Serialize(PacketInfo& info) {
		std::vector<uint8_t> buffer;
		PacketScript::Encode4(buffer, info.PID);
		PacketScript::Encode4(buffer, info.Index);
		PacketScript::Encode1(buffer, info.IsInPacket);
		PacketScript::Encode4(buffer, info.Payload.size());
		buffer.insert(buffer.end(), info.Payload.begin(), info.Payload.end());
		PacketScript::Encode4(buffer, info.Actions.size());
		for (const PacketAction& action : info.Actions) {
			PacketScript::Encode1(buffer, uint8_t(action.Type));
			PacketScript::Encode4(buffer, action.Size);
			PacketScript::Encode4(buffer, action.RetAddr);
		}
		return buffer;
	}

	static PacketInfo Deserialize(std::vector<uint8_t>& buffer) {
		PacketInfo info{};
		size_t pos = 0;
		info.PID = PacketScript::Decode4(buffer, pos);
		info.Index = PacketScript::Decode4(buffer, pos);
		info.IsInPacket = PacketScript::Decode1(buffer, pos);
		size_t payloadSize = PacketScript::Decode4(buffer, pos);
		info.Payload.assign(buffer.begin() + pos, buffer.begin() + pos + payloadSize);
		pos += payloadSize;
		size_t actionSize = PacketScript::Decode4(buffer, pos);
		for (size_t i = 0; i < actionSize; i++)
		{
			PacketAction action{
				PacketActionType(PacketScript::Decode1(buffer,pos)),
				PacketScript::Decode4(buffer, pos),
				PacketScript::Decode4(buffer, pos),
			};
			info.Actions.push_back(action);
		}
		return info;
	}
}
