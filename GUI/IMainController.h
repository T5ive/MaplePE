#pragma once
#include "Model.h"

class IMainController {
public:
	virtual const Setting& GetSetting() = 0;
	virtual bool SetSetting(const Setting& s) = 0;
	virtual std::wstring GetExeDir() = 0;
	virtual void OnPacketLogModel(PacketLogModel& log) = 0;
	virtual PacketLogModel GetPacketLogModel(int logID) = 0;
	virtual const std::vector<PacketLogModel>& GetPacketLogModels() = 0;
	virtual bool JumpLogItem(int logID) = 0;
	virtual std::wstring SendFormatData(int logID, const std::wstring& data) = 0;
	virtual ~IMainController() = default;
};