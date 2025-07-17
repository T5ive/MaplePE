#pragma once
#include "IMainController.h"
#include "LoggingServer.h"

class MainView;
class LoggingServer;

class MainController final : public IMainController {
public:
	MainController(MainView* view);
	~MainController();
	const Setting& GetSetting() override;
	bool SetSetting(const Setting& s) override;
	std::wstring GetExeDir() override;
	void OnPacketLogModel(PacketLogModel& log) override;
	PacketLogModel GetPacketLogModel(int logID) override;
	const std::vector<PacketLogModel>& GetPacketLogModels() override;
	bool JumpLogItem(int logID) override;
	std::wstring SendData(const int pid, const bool isInPacket, const std::wstring& data);
	std::wstring SendFormatData(int logID, const std::wstring& data) override;
	void ClearPacketLogModel();
private:
	Setting m_setting;

	std::wstring m_exeDir = L"";
	MainView* m_mainView = nullptr;
	LoggingServer* m_loggingServer = nullptr;
	std::vector<PacketLogModel> m_packetLogModels;
};