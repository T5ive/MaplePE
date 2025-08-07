// GenCodeView.cpp : implementation file
//
#include "pch.h"
#include "GenCodeView.h"
#include "resource.h"

// GenCodeView dialog

IMPLEMENT_DYNAMIC(GenCodeView, CDialogEx)

GenCodeView::GenCodeView(IFormatController* formatControllerImpl, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GEN_CODE_DIALOG, pParent), BaseView(L"CenCodeView")
{
	m_formatControllerImpl = formatControllerImpl;
}

GenCodeView::~GenCodeView()
{
}

void GenCodeView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GEN_CODE_EDIT, m_genCodeEdit);
}

BOOL GenCodeView::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	this->SetHWND(this->GetSafeHwnd());
	std::vector<std::wstring> codes;
	m_formatControllerImpl->GenCodes(codes);
	std::wstring fullText;
	for (const auto& line : codes) {
		fullText += line + L"\r\n";
	}
	m_genCodeEdit.SetWindowTextW(fullText.c_str());
	return TRUE;
}

void GenCodeView::PostNcDestroy()
{
	delete(this);
}

BEGIN_MESSAGE_MAP(GenCodeView, CDialogEx)
END_MESSAGE_MAP()


// GenCodeView message handlers