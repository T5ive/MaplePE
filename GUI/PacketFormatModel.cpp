#include "PacketFormatModel.h"

PacketFormatModel::PacketFormatModel(size_t index, std::wstring retAddr, uint8_t actionType, std::wstring actionText, size_t size, std::wstring value, std::wstring segment)
{
	m_index = index;
	m_retAddr = retAddr;
	m_action = actionType;
	m_actionText = actionText;
	m_size = size;
	m_value = value;
	m_segment = segment;
}

PacketFormatModel::~PacketFormatModel()
{
}

const size_t PacketFormatModel::GetIndex() const
{
	return m_index;
}

const std::wstring PacketFormatModel::GetRetAddr() const
{
	return m_retAddr;
}

const uint8_t PacketFormatModel::GetAction() const
{
	return m_action;
}

const std::wstring PacketFormatModel::GetActionText() const
{
	return m_actionText;
}

const size_t PacketFormatModel::GetSize() const
{
	return m_size;
}

const std::wstring PacketFormatModel::GetValue() const
{
	return m_value;
}

const std::wstring PacketFormatModel::GetSegment() const
{
	return m_segment;
}

const std::wstring PacketFormatModel::GetComment() const
{
	return m_comment;
}

void PacketFormatModel::SetSize(size_t newSize)
{
	m_size = newSize;
}

void PacketFormatModel::SetValue(const std::wstring& value)
{
	m_value = value;
}

void PacketFormatModel::SetSegment(const std::wstring& segment)
{
	m_segment = segment;
}

void PacketFormatModel::SetComment(const std::wstring& comment)
{
	m_comment = comment;
}

