#pragma once
#include "framework.h"
#include "Model.h"
#include "BaseView.h"
#include "IMainController.h"
#include "PatchController.h"

// PatchView dialog

class PatchView final : public CDialogEx, public BaseView
{
	DECLARE_DYNAMIC(PatchView)

public:
	PatchView(IMainController* mainControllerImpl, CWnd* pParent = nullptr);   // standard constructor
	~PatchView();

private:
	PatchController* m_patchController = nullptr;

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PATCH_DIALOG };
#endif

protected:
	CEdit m_pidEdit;
	
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support
	BOOL OnInitDialog() override;

	// Generated message map functions
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedInjectDllButton();
};
