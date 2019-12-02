
// TestServer.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "TestServer.h"
#include "TestServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestServerApp

BEGIN_MESSAGE_MAP(CTestServerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTestServerApp 생성

CTestServerApp::CTestServerApp()
{
	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CTestServerApp 개체입니다.

CTestServerApp theApp;


// CTestServerApp 초기화

BOOL CTestServerApp::InitInstance()
{
	
	WSADATA  temp;
	WSAStartup(0x0202, &temp);

	CTestServerDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	
	WSACleanup();

	return FALSE;
}

