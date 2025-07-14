#include "GUI.h"
#include "MainView.h"

MainApp theApp;

BOOL MainApp::InitInstance() {
	CWinApp::InitInstance();

	MainView mainView;
	m_pMainWnd = &mainView;
	mainView.DoModal();

	return FALSE;
}