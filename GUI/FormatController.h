#pragma once
#include "PacketFormatModel.h"
#include "IMainController.h"
#include "IFormatController.h"

class FormatView;

class FormatController final : public IFormatController {
public:
	FormatController(int nSelectedID, IMainController* mainControllerImpl, FormatView* view);
	~FormatController();
	int GetLogID();
	const std::vector<PacketFormatModel>& GetPacketFormatModels();
	bool EncodeValue(int row, const std::wstring& text, std::wstring& segment, std::wstring& strLen);
	bool DecodeSegment(int row, const std::wstring& text, std::wstring& value, std::wstring& strLen);
	bool UpdateCommnet(int row, const std::wstring& text);
	void GenCodes(std::vector<std::wstring>& codes) override;
	std::wstring SendFormatData();
private:
	int m_logID = 0;
	FormatView* m_formatView = nullptr;
	IMainController* m_mainControllerImpl = nullptr;
	std::vector<PacketFormatModel> m_packetFormatModels;

	void initPacketFormatModels();
	PacketFormatModel* getPacketFormatModel(int index);
};