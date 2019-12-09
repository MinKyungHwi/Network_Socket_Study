
// TestServerDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "TestServer.h"
#include "TestServerDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTestServerDlg 대화 상자



CTestServerDlg::CTestServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TESTSERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_client_count = 0;
}

void CTestServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_event_list);
}

BEGIN_MESSAGE_MAP(CTestServerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// CTestServerDlg 메시지 처리기

BOOL CTestServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	
	//////////////////////////////////////////////////////////

	mh_listen_socket = socket(AF_INET, SOCK_STREAM, 0); //SOCK_STREAM :TCP , SOCK_DGRAM : UDP
	//socket(AF_INET, SOCK_STREAM, 0); AF_INET = IPv4 ,SOCK_STREAM :TCP , 0 : 프로토콜 인증 지금경우 AF_INET가 이미 지정되어 있어서 값을 입력할 필요는 없다)

	sockaddr_in srv_addr;

	srv_addr.sin_family = AF_INET;
	srv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	srv_addr.sin_port = htons(20001); //htons() 다른 OS 사용하는 바이트 변환기법이 달라서 오류가 날수있지만 해당 함수를 사용해서 제거를 한다 

	bind(mh_listen_socket, (SOCKADDR*)&srv_addr, sizeof(srv_addr));


	listen(mh_listen_socket, 1); //대기자를 받을수 있는 수 (1의 의미) ex) 
	AddEventString(L"클라이언트 접속을 허락합니다...");
	//accept() 클라이언트 접속했는지 알수있다. 단)클라이언트 접속안하면 응답없음으로 빠짐  =  동기방식(스레드 이용)/비동기방식이 있음)
	
	//mh_listen_socket 소켓에 사용자가 접속행위를 발견하면(FD_ACCEPT)  m_hWnd로 27001으로 들어옴)
	WSAAsyncSelect(mh_listen_socket, m_hWnd, 27001, FD_ACCEPT);

	

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

//////////////////////////////////////////////////////////


// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CTestServerDlg::OnPaint()
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
HCURSOR CTestServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


/////////////////////////////////////추가 코드

void CTestServerDlg::AcceptProcess(SOCKET parm_h_socket) {
	if (MAX_CLIENT_COUNT > m_client_count) {
		
		sockaddr_in client_addr;
		int sockaddr_in_size = sizeof(client_addr);

		mh_client_list[m_client_count] = accept(parm_h_socket, (SOCKADDR*)&client_addr, &sockaddr_in_size);
		
		WSAAsyncSelect(mh_client_list[m_client_count], m_hWnd, 27002, FD_READ | FD_CLOSE);
		
		//클라이언트 주소를 알고 싶을때 
		CString ip_address;
		wcscpy(m_client_ip[m_client_count], ip_address);
		ip_address = inet_ntoa(client_addr.sin_addr);

		AddEventString(L"새로운 클라이언트가 접속을 했습니다.:" + ip_address);
				
		m_client_count++;
	}
	else {

	}
 }

void CTestServerDlg::AddEventString(CString parm_string) {
	int index = m_event_list.InsertString(-1, parm_string);
	m_event_list.SetCurSel(index);
}

void CTestServerDlg::ClientCloseProcess(SOCKET parm_h_socket, char parm_force_flag) {
	if (parm_force_flag == 1) {
		LINGER temp_linger = { TRUE,0 };	//소켓을 끊을때 어떻게 끊을것인가 명시
		setsockopt(parm_force_flag, SOL_SOCKET, SO_LINGER, (char*)&temp_linger, sizeof(&temp_linger));
	}
	closesocket(parm_h_socket);

	for (int i = 0; i < m_client_count; i++) {
		if (mh_client_list[i] == parm_h_socket) {
			m_client_count--;
			if (i != m_client_count) {
				mh_client_list[i] = mh_client_list[m_client_count];
				wcscpy(m_client_ip[i], m_client_ip[m_client_count]);
			}
		}
	}
}


void CTestServerDlg::ReceiveData(SOCKET parm_hsocket, char *parm_p_buffer, int parm_size){
	int current_size, total_size = 0, retry_count = 0;
	while (total_size < parm_size) {
		current_size = recv(parm_hsocket, parm_p_buffer + total_size, parm_size - total_size, 0);

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
LRESULT CTestServerDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// wParam 핸들 값   ,  lParam : 오류확인
	if (27001 == message) {
		AcceptProcess((SOCKET)wParam);
	} else if(27002 == message) { //FD_READ , FD_CLOSE
		SOCKET h_socket = (SOCKET)wParam; //mh_listen_socket 핸들값과 동일
		if (WSAGETSELECTEVENT(lParam) == FD_READ) {
			WSAAsyncSelect(h_socket, m_hWnd, 27002, FD_CLOSE);
			
			char key, network_message_id;
			recv(h_socket, &key, 1, 0);
			if (key == 27) {
				int current_size, total_size = 0 , retry_count = 0;
				unsigned short int body_size;
				recv(h_socket, (char*)&body_size, sizeof(body_size), 0);
				recv(h_socket, &network_message_id, 1, 0);

				if (body_size > 0) {
					char* p_body_data = new char[body_size];

					ReceiveData(h_socket, p_body_data, body_size);

					if (network_message_id == 1) {
						// 실제로 클라이언트가 보내준 데이터를 처리( p_body_data)
					}
					delete[] p_body_data;
				}

				WSAAsyncSelect(h_socket, m_hWnd, 27002, FD_CLOSE | FD_READ) ;
			}else ClientCloseProcess(h_socket,1);
		}else ClientCloseProcess(h_socket,0); // FD_CLOSE
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}