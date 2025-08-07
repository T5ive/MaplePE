#pragma once
#include <vector>

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

	std::vector<uint8_t> Serialize(PacketInfo& info);

	PacketInfo Deserialize(std::vector<uint8_t>& buffer);
}
