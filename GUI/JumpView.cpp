// JumpView.cpp : implementation file
//

#include "pch.h"
#include "JumpView.h"
#include "resource.h"

// JumpView dialog

IMPLEMENT_DYNAMIC(JumpView, CDialogEx)

JumpView::JumpView(IMainController* mainControllerImpl, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_JUMP_DIALOG, pParent), BaseView(L"JumpView")
{
	m_mainControllerImpl = mainControllerImpl;
}

JumpView::~JumpView()
{
}

void JumpView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_JUMP_ID_EDIT, m_jumpIDEdit);
}

BOOL JumpView::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	m_jumpIDEdit.ModifyStyle(0, ES_CENTER);
	m_jumpIDEdit.SetWindowTextW(m_lastJumpID);
	return TRUE;
}


BEGIN_MESSAGE_MAP(JumpView, CDialogEx)
	ON_BN_CLICKED(IDC_JUMP_BUTTON, &JumpView::OnBnClickedJumpButton)
END_MESSAGE_MAP()


// JumpView message handlers

void JumpView::OnBnClickedJumpButton()
{
	// TODO: Add your control notification handler code here
	m_jumpIDEdit.GetWindowTextW(m_lastJumpID);
	int logID = _wtoi(m_lastJumpID);
	bool ok = m_mainControllerImpl->JumpLogItem(logID);
	if (!ok) {
		MBError(L"Failed to jump to log item");
		return;
	}
	EndDialog(0);
}