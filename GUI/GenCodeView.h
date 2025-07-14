#pragma once
#include "framework.h"
#include "BaseView.h"
#include "IFormatController.h"

// GenCodeView dialog

class GenCodeView : public CDialogEx, public BaseView
{
	DECLARE_DYNAMIC(GenCodeView)

public:
	GenCodeView(IFormatController* formatControllerImpl,CWnd* pParent = nullptr);   // standard constructor
	virtual ~GenCodeView();
private:
	IFormatController* m_formatControllerImpl = nullptr;
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GEN_CODE_DIALOG };
#endif

protected:
	CEdit m_genCodeEdit;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL OnInitDialog() override;
	void PostNcDestroy() override;
	DECLARE_MESSAGE_MAP()
};
