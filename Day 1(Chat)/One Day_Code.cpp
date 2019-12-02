#include <WinSock2.h>  				//Socket API 추가 
#pragma comment (lib, "ws2_32.lib")	//전처리기를 통해  ws2_32.lib 추가

 	
	 //★☆ 헤더파일  ★☆// 
	 
	 SOCKET mh_listen_socket;
	 mh_listen_socket = socket(AF_INET, SOCK_STREAM, 0); 
	 
	//socket(AF_INET, SOCK_STREAM, 0); 
	//AF_INET = IPv4 
	//SOCK_STREAM :TCP 
	// 0 : 프로토콜 지정값
	 
	 //★☆ 헤더 끝  ★☆// 
	 
	 
	 
	 
	 //	★☆cpp 파일 ★☆//
	  
	
	//소켓 생성 
	sockaddr_in srv_addr; 							
	srv_addr.sin_family = AF_INET;					
	srv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//htons() OS 마다 사용하는 정수를 변환 하는게 달라서 오류가 발생하는걸 방지
	srv_addr.sin_port = htons(20001); 


	//소켓정보 입력 
	bind(mh_listen_socket, (SOCKADDR*)&srv_addr, sizeof(srv_addr));
	
	//
	listen(mh_listen_socket, 1); //대기자를 받을수 있는 수 (1의 의미) ex) 

	//accept() 클라이언트 접속했는지 알수있다. 단)클라이언트 접속안하면 응답없음으로 빠짐  =  동기방식(스레드 이용)/비동기방식이 있음)
	//mh_listen_socket 소켓에 사용자가 접속행위를 발견하면(FD_ACCEPT)  m_hWnd로 27001으로 들어옴)
	WSAAsyncSelect(mh_listen_socket, m_hWnd, 27001, FD_ACCEPT);
	
	
	
	LRESULT CTestServerDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// wParam 핸들 값   ,  lParam : 오류확인
	if (27001 == message) {
		if (MAX_CLIENT_COUNT > m_client_cout) {
			SOCKET h_socket = (SOCKET)wParam; //mh_listen_socket 핸들값과 동일

			sockaddr_in client_addr;
			int sockaddr_in_size = sizeof(client_addr);

			mh_client_list[m_client_cout] = accept(h_socket, (SOCKADDR*)&client_addr, &sockaddr_in_size);
			WSAAsyncSelect(mh_client_list[m_client_cout], m_hWnd, 27002, FD_READ | FD_CLOSE);
			m_client_cout++;

			CString ip_address;
			ip_address = inet_ntoa(client_addr.sin_addr);

			MessageBox(ip_address, L"새로운 클라이언트가 접속을 했습니다.:", MB_OK);
		}
		else {

		}
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}
	
	//	★☆cpp 파일 끝 ★☆//
	
	
	
