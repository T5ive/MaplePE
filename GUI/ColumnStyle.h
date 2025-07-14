#pragma once
#include <vector>
#include <string>

static std::vector<int> GetColumnWidths(int totalWidth,const std::vector<float>& ratios) {
	size_t columnLength = ratios.size();
	std::vector<int> columnWidths(columnLength);
	int usedWidth = 0;
	for (size_t i = 0; i< columnLength; i++)
	{
		int columnWidth = 0;
		if (i == columnLength - 1) {
			columnWidth = totalWidth - usedWidth;
		}
		else {
			columnWidth = int(totalWidth * ratios[i]);
		}
		columnWidths[i] = columnWidth;
		usedWidth += columnWidth;
	}
	return columnWidths;
}

#pragma region MainView & SearchView
const std::vector<std::wstring> kLogColumnTitle = {
	L"ID",
	L"PID",
	L"Index",
	L"Type",
	L"Length",
	L"Opcode",
	L"Data",
};

enum class kLogColumnType {
	ID,
	PID,
	Index,
	Type,
	Length,
	Opcode,
	Data,
};

const std::vector<float> kLogColumnWidthRatio = {
	0.1f,
	0.1f,
	0.1f,
	0.1f,
	0.1f,
	0.1f,
	0.4f,
};
#pragma endregion

#pragma region FormatView
const std::vector<std::wstring> kFormatColumnTitle = {
	L"Index",
	L"RetAddr",
	L"Action",
	L"Size",
	L"Value",
	L"Segment",
	L"Comment",
};

enum class kFormatColumnType {
	Index,
	RetAddr,
	Action,
	Size,
	Value,
	Segment,
	Comment,
};

const std::vector<float> kFormatColumnWidthRatio = {
	0.1f,
	0.15f,
	0.15f,
	0.05f,
	0.2f,
	0.2f,
	0.15f,
};
#pragma endregion