#pragma once
#include "BaseView.h"
#include "IMainController.h"

// JumpView dialog

class JumpView : public CDialogEx, public BaseView
{
	DECLARE_DYNAMIC(JumpView)

public:
	JumpView(IMainController* mainControllerImpl, CWnd* pParent = nullptr);   // standard constructor
	virtual ~JumpView();
private:
	CString m_lastJumpID;
	IMainController* m_mainControllerImpl = nullptr;
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_JUMP_DIALOG };
#endif

protected:
	CEdit m_jumpIDEdit;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL OnInitDialog() override;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedJumpButton();
};
