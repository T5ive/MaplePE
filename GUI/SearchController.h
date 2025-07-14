#pragma once
#include "IMainController.h"

class SearchView;

class SearchController final {
public:
	SearchController(IMainController* mainControllerImpl, SearchView* view);
	~SearchController();
	const std::vector<std::wstring> GetSearchConditionText();
	std::vector<std::pair<int, PacketLogModel>> SearchPacketLog(int condition, int pid, const std::wstring& searchText);
	void JumpLogItem(int logID);
private:
	SearchView* m_searchView = nullptr;
	IMainController* m_mainControllerImpl = nullptr;
};