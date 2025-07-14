#include "framework.h"
#include "BaseView.h"


BaseView::BaseView(std::wstring title)
{
	m_title = title;
}

BaseView::~BaseView()
{

}

void BaseView::SetHWND(HWND hWnd)
{
	m_hWnd = hWnd;
}

void BaseView::SetDialogTitleWithID(int id)
{
	wchar_t originalTitle[256] = { 0 };
	GetWindowTextW(m_hWnd, originalTitle, 256);
	wchar_t newTitle[512];
	swprintf_s(newTitle, 512, L"%s [ID: %d]", originalTitle, id);
	SetWindowTextW(m_hWnd, newTitle);
}

void BaseView::MBInfo(const std::wstring& text)
{
	MessageBox(m_hWnd, text.c_str(), m_title.c_str(), MB_OK);
}

void BaseView::MBWarn(const std::wstring& text)
{
	MessageBox(m_hWnd, text.c_str(), m_title.c_str(), MB_ICONWARNING | MB_OK);
}

void BaseView::MBError(const std::wstring& text)
{
	MessageBox(m_hWnd, text.c_str(), m_title.c_str(), MB_ICONERROR | MB_OK);
}

bool BaseView::MBYesNo(const std::wstring& text)
{
	return MessageBox(m_hWnd, text.c_str(), m_title.c_str(), MB_YESNO | MB_ICONWARNING) == IDYES;
}

AnchorInfo BaseView::GetAnchorInfo(CWnd* pCtrl)
{
	AnchorInfo anchor{};
	if (pCtrl && pCtrl->GetSafeHwnd()) {
		CWnd* pParent = pCtrl->GetParent();
		if (!pParent || !pParent->GetSafeHwnd()) {
			return anchor;
		}
		CRect clientRect, ctrlRect;
		pParent->GetClientRect(&clientRect);
		pCtrl->GetWindowRect(&ctrlRect);
		pParent->ScreenToClient(&ctrlRect);

		int offsetX = clientRect.right - ctrlRect.right;
		int offsetY = clientRect.bottom - ctrlRect.bottom;

		anchor.OffsetXRatio = static_cast<float>(offsetX) / clientRect.Width();
		anchor.OffsetYRatio = static_cast<float>(offsetY) / clientRect.Height();
		anchor.Width = ctrlRect.Width();
		anchor.Height = ctrlRect.Height();
	}
	return anchor;
}

void BaseView::UpdateCtrlPosition(CWnd* pCtrl, int cx, int cy, const AnchorInfo& anchorInfo)
{
	if (pCtrl && pCtrl->GetSafeHwnd()) {
		CWnd* pParent = pCtrl->GetParent();
		if (!pParent || !pParent->GetSafeHwnd()) {
			return;
		}
		CRect ctrlRect;
		pCtrl->GetWindowRect(&ctrlRect);
		pParent->ScreenToClient(&ctrlRect);
		int offsetX = static_cast<int>(cx * anchorInfo.OffsetXRatio);
		int offsetY = static_cast<int>(cy * anchorInfo.OffsetYRatio);
		int newX = cx - anchorInfo.Width - offsetX;
		int newY = cy - anchorInfo.Height - offsetY;
		pCtrl->MoveWindow(newX, newY, anchorInfo.Width, anchorInfo.Height);
	}
}
