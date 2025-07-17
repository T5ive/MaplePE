#include "PacketLogModel.h"



PacketLogModel::PacketLogModel(PacketInfo& info)
{
	m_pid = info.PID;
	m_index = info.Index;
	m_isInPacket = info.IsInPacket;
	m_length = info.Payload.size();
	size_t pos = 0;
	m_opcode = PacketScript::Decode2(info.Payload, pos);
	PacketScript::Buffer2Data(info.Payload, m_data);
	m_actions.assign(info.Actions.begin(), info.Actions.end());
}

PacketLogModel::PacketLogModel(int pid, bool isInPacket, const std::wstring& data)
{
	m_pid = pid;
	m_isInPacket = isInPacket;
	m_data = data;
}

const bool PacketLogModel::IsEmpty() const
{
	return m_pid == 0;
}

const int PacketLogModel::GetPID() const
{
	return static_cast<int>(m_pid);
}

const std::wstring PacketLogModel::GetPIDStr() const
{
	return std::to_wstring(m_pid);
}

const std::wstring PacketLogModel::GetIndexStr() const
{
	return std::to_wstring(m_index);
}

const std::wstring PacketLogModel::GetType() const
{
	return m_isInPacket ? L"In" : L"Out";
}

const bool PacketLogModel::IsInPacket() const
{
	return m_isInPacket;
}

const std::wstring PacketLogModel::GetLengthStr() const {
	return std::to_wstring(m_length);
}

const std::wstring PacketLogModel::GetOpcodeStr() const
{
	if (m_isTypeHeader1Byte) {
		uint8_t firstByte = static_cast<uint8_t>(m_opcode & 0xFF);
		return std::to_wstring(firstByte);
	}
	return std::to_wstring(m_opcode);
}

const std::wstring& PacketLogModel::GetData() const {
	return m_data;
}

const std::vector<PacketAction>& PacketLogModel::GetActions() const
{
	return m_actions;
}

void PacketLogModel::SetIsTypeHeader1Byte(bool isTypeHeader1Byte)
{
	m_isTypeHeader1Byte = isTypeHeader1Byte;
}


void PacketLogModel::SetData(const std::wstring& data)
{
	m_data = data;
}
