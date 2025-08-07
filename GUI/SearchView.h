#pragma once
#include "BaseView.h"
#include "IMainController.h"
#include "SearchController.h"

// SearchView dialog

class SearchView : public CDialogEx, public BaseView
{
	DECLARE_DYNAMIC(SearchView)

public:
	SearchView(IMainController* mainControllerImpl, CWnd* pParent = nullptr);   // standard constructor
	virtual ~SearchView();
private:
	SearchController* m_searchController = nullptr;
	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SEARCH_DIALOG };
#endif

protected:
	CComboBox m_searchConditionComboBox;
	CEdit m_searchPIDEdit;
	CEdit m_searchTextEdit;
	CListCtrl m_packetLogSearchListCtrl;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL OnInitDialog() override;
	void PostNcDestroy() override;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedFindButton();
	afx_msg void OnNMDblclkPacketLogList(NMHDR* pNMHDR, LRESULT* pResult);
};
