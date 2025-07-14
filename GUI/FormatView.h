#pragma once
#include "framework.h"
#include "Model.h"
#include "BaseView.h"
#include "GenCodeView.h"
#include "GridCtrl.h"
#include "IMainController.h"
#include "FormatController.h"

// FormatView dialog

class FormatView final : public CDialogEx, public BaseView
{
	DECLARE_DYNAMIC(FormatView)

public:
	FormatView(int nSelectedID, IMainController* mainControllerImpl, CWnd* pParent = nullptr);   // standard constructor
	virtual ~FormatView();

private:
	FormatController* m_formatController = nullptr;
	GenCodeView* m_genCodeView = nullptr;
	AnchorInfo m_genCodeButtonAnchorInfo{};
	AnchorInfo m_sendButtonAnchorInfo{};

	void UpdateCellWidth();

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FORMAT_DIALOG };
#endif

protected:
	CGridCtrl m_packetFormatGridCtrl;
	CString m_prevCellText;

	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support
	BOOL OnInitDialog() override;
	void PostNcDestroy() override;

	// Generated message map functions
	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGridCellBeginEdit(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnGridCellEdited(NMHDR* pNotifyStruct, LRESULT* pResult);
	afx_msg void OnBnClickedGenCodeButton();
	afx_msg void OnBnClickedSendButton();
};
