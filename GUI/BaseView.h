#pragma once
#include <Windows.h>
#include <string>

struct AnchorInfo {
	float OffsetXRatio;
	float OffsetYRatio;
	int Width;
	int Height;
};

class BaseView {
public:
	BaseView(std::wstring title);
	~BaseView();
	void SetHWND(HWND hWnd);
	void SetDialogTitleWithID(int id);
	void MBInfo(const std::wstring& text);
	void MBWarn(const std::wstring& text);
	void MBError(const std::wstring& text);
	bool MBYesNo(const std::wstring& text);
	AnchorInfo GetAnchorInfo(CWnd* pCtrl);
	void UpdateCtrlPosition(CWnd* pCtrl, int cx, int cy, const AnchorInfo& anchorInfo);

private:
	HWND m_hWnd = 0;
	std::wstring m_title = L"";
};