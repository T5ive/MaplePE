#pragma once
#include "framework.h"
#include "BaseView.h"
#include "IMainController.h"
#include "SettingController.h"

// SettingView dialog

class SettingView : public CDialogEx, public BaseView
{
	DECLARE_DYNAMIC(SettingView)

public:
	SettingView(IMainController* mainControllerImpl, CWnd* pParent = nullptr);   // standard constructor
	virtual ~SettingView();

private:
	SettingController* m_settingController = nullptr;

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTING_DIALOG };
#endif

protected:
	CMFCPropertyGridCtrl m_settingPropGridCtrl;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL OnInitDialog() override;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSaveSettingButton();
	afx_msg void OnBnClickedResetSettingButton();
};
