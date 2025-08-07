#include "pch.h"
#include "MainView.h"
#include "MainController.h"

MainController::MainController(MainView* view) {
	m_mainView = view;
	m_exeDir = GetExecutableDir(NULL);
	m_loggingServer = new LoggingServer(this);
	m_loggingServer->Run();

	bool loadOK = LoadSetting(m_exeDir, m_setting);
	if (!loadOK) {
		bool saveOK = SaveSetting(m_exeDir, m_setting);
		if (!saveOK) {
			m_mainView->MBError(L"Failed to save setting on first load");
		}
	}
}

MainController::~MainController()
{
	delete(m_loggingServer);
	m_loggingServer = nullptr;
}

const Setting& MainController::GetSetting()
{
	return m_setting;
}

bool MainController::SetSetting(const Setting& s)
{
	bool ok = SaveSetting(m_exeDir, s);
	if (!ok) {
		return false;
	}
	m_setting = s;
	return true;
}

std::wstring MainController::GetExeDir()
{
	return m_exeDir;
}

void MainController::OnPacketLogModel(PacketLogModel& log)
{
	log.SetIsTypeHeader1Byte(m_setting.IsTypeHeader1Byte);
	m_packetLogModels.push_back(log);
	m_mainView->InsertPacketLogItem(m_packetLogModels.size() - 1, log);
}

PacketLogModel MainController::GetPacketLogModel(int logID)
{
	if (logID < 0 || static_cast<size_t>(logID) >= m_packetLogModels.size()) {
		std::wstring err = L"Failed to get packet log model with logID " + std::to_wstring(logID);
		this->m_mainView->MBError(err);
		return PacketLogModel{};
	}
	return m_packetLogModels[logID];
}

const std::vector<PacketLogModel>& MainController::GetPacketLogModels()
{
	return m_packetLogModels;
}

bool MainController::JumpLogItem(int logID)
{
	if (logID < 0 || static_cast<size_t>(logID) >= m_packetLogModels.size()) {
		return false;
	}
	m_mainView->JumpLogItem(logID);
	return true;
}

std::wstring MainController::SendData(const int pid, const bool isInPacket, const std::wstring& data)
{
	PacketLogModel log(pid, isInPacket, data);
	if (pid == 0) {
		return m_loggingServer->BroadcastPacketInfo(log);
	}
	return m_loggingServer->NotifyPacketInfo(log);
}

std::wstring MainController::SendFormatData(int logID, const std::wstring& data)
{
	PacketLogModel log = this->GetPacketLogModel(logID);
	log.SetData(data);
	return m_loggingServer->NotifyPacketInfo(log);
}

void MainController::ClearPacketLogModel()
{
	m_packetLogModels.clear();
}