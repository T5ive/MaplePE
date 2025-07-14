#pragma once
#include <string>
class PacketFormatModel {
public:
	PacketFormatModel() = default;
	PacketFormatModel(size_t index, std::wstring retAddr, uint8_t actionType, std::wstring actionText,
		size_t size, std::wstring value, std::wstring segment);
	~PacketFormatModel();
	const size_t GetIndex() const;
	const std::wstring GetRetAddr() const;
	const uint8_t GetAction() const;
	const std::wstring GetActionText() const;
	const size_t GetSize() const;
	const std::wstring GetValue()const;
	const std::wstring GetSegment()const;
	const std::wstring GetComment()const;
	void SetSize(size_t newSize);
	void SetValue(const std::wstring& value);
	void SetSegment(const std::wstring& segment);
	void SetComment(const std::wstring& comment);
private:
	size_t m_index = 0;
	std::wstring m_retAddr;
	uint8_t m_action = 0;
	std::wstring m_actionText;
	size_t m_size = 0;
	std::wstring m_value;
	std::wstring m_segment;
	std::wstring m_comment;
};
