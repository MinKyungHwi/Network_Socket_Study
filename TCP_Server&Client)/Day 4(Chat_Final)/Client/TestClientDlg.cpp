
// TestClientDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "TestClient.h"
#include "TestClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestClientDlg 대화 상자



CTestClientDlg::CTestClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TESTCLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_event_list);
}

BEGIN_MESSAGE_MAP(CTestClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SEND_BTN, &CTestClientDlg::OnBnClickedSendBtn)
	ON_BN_CLICKED(IDOK, &CTestClientDlg::OnBnClickedOk)
END_MESSAGE_MAP()

void CTestClientDlg::AddEventString(CString parm_string) {

	int index = m_event_list.InsertString(-1, parm_string); //addstring , insertstring
	m_event_list.SetCurSel(index);
}

// CTestClientDlg 메시지 처리기

BOOL CTestClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.


	mh_socket = socket(AF_INET, SOCK_STREAM, 0);

	sockaddr_in srv_addr;
	
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	srv_addr.sin_port = htons(20001);


	WSAAsyncSelect(mh_socket, m_hWnd, 27001, FD_CONNECT);
	connect(mh_socket, (sockaddr*)&srv_addr, sizeof(srv_addr)); //서버가 on이면 바로 가능 , off면 대기 해야함


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CTestClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CTestClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTestClientDlg::ConnectProcess(LPARAM lParam) 
{
	if (WSAGETSELECTERROR(lParam) == 0) {
		WSAAsyncSelect(mh_socket, m_hWnd, 27002, FD_READ | FD_CLOSE);
		AddEventString("서버에 접속했습니다.");
	}
	else {
		DestroySocket();
		AddEventString("서버에 접속할 수 없습니다.");
	}
}


void CTestClientDlg::DestroySocket()
{
	LINGER temp_linger = { TRUE,0 };
	setsockopt(mh_socket, SOL_SOCKET, SO_LINGER, (char*)&temp_linger, sizeof(temp_linger));
	closesocket(mh_socket);
	mh_socket = INVALID_SOCKET;
	AddEventString("잘못된 프로토콜입니다.");
}

void CTestClientDlg::ReceiveData(char* parm_p_buffer, int parm_size) {
	int current_size, total_size = 0, retry_count = 0;
	while (total_size < parm_size) {
		current_size = recv(mh_socket, parm_p_buffer + total_size, parm_size - total_size, 0);

		if (current_size == SOCKET_ERROR) {
			retry_count++;
			Sleep(50);
			if (retry_count > 5) break;

		}
		else {
			retry_count = 0;
			total_size = total_size + current_size;
		}
	}
}

void CTestClientDlg::ReadFrameData()
{
	char key , message_id;
	recv(mh_socket, &key, 1, 0);
	if (key == 27) {
		unsigned short int body_size;
		recv(mh_socket, (char*)&body_size, 2, 0);
		recv(mh_socket, &message_id, 1, 0);
		if (body_size > 0) {
			char* p_body_data = new char[body_size];

			ReceiveData(p_body_data, body_size);

			if (message_id == 1) {
				AddEventString(p_body_data);
			}

			delete[] p_body_data;
		}
	}
	else {
		DestroySocket();
		AddEventString("잘못된 프로토콜입니다.");
	}
}

LRESULT CTestClientDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if (message == 27001) { // FD_CONNECT
		ConnectProcess(lParam);
	}
	else if (message == 27002) { // FD_READ , FD_CLOSE
		if (WSAGETSELECTEVENT(lParam) == FD_READ) {
			WSAAsyncSelect(mh_socket, m_hWnd, 27002, FD_CLOSE);

			ReadFrameData();
			
			if(mh_socket != INVALID_SOCKET) WSAAsyncSelect(mh_socket, m_hWnd, 27002, FD_CLOSE | FD_READ);
		}
		else {
			DestroySocket();
			AddEventString("서버에 연결을 해제하였습니다.");
		}
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}

void CTestClientDlg::SendFrameData(SOCKET param_h_socket, unsigned char param_id, const void* parm_p_data, int param_size)
{
	char* p_send_data = new char[param_size +4];

	*p_send_data = 27;
	*(unsigned short int *)(p_send_data + 1 ) = param_size;
	*(p_send_data + 3) = param_id;

	memcpy(p_send_data + 4, parm_p_data, param_size);

	send(param_h_socket, p_send_data, param_size + 4, 0);

	delete[] p_send_data;
}


void CTestClientDlg::OnBnClickedSendBtn()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (mh_socket != INVALID_SOCKET) {
		CString str;
		GetDlgItemText(IDC_EDIT1, str);

		SendFrameData(mh_socket, 1, (const wchar_t*)&str, (str.GetLength() + 1) * 2);
	}
	else {
		AddEventString("서버에 접속된 상태가 아닙니다. ");
	}


}


void CTestClientDlg::OnBnClickedOk()
{
	OnBnClickedSendBtn();
	GotoDlgCtrl(GetDlgItem(IDC_EDIT1));

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// CDialogEx::OnOK();
}
