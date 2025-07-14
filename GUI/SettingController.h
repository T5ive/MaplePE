#pragma once
#include "IMainController.h"

class SettingView;

class SettingController final {
public:
	SettingController(IMainController* mainControllerImpl, SettingView* view);
	~SettingController();
	const std::vector<std::wstring> Deserialize(const Setting& s);
	const Setting Serialize(const std::vector<std::wstring>& propValues);
	const std::vector<std::wstring> GetSettings();
	bool SetSetting(const std::vector<std::wstring> propValues);
	const std::vector<std::wstring> ResetSetting();
private:
	SettingView* m_settingView = nullptr;
	IMainController* m_mainControllerImpl = nullptr;
};