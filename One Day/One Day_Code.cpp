#include <WinSock2.h>  				//Socket API �߰� 
#pragma comment (lib, "ws2_32.lib")	//��ó���⸦ ����  ws2_32.lib �߰�

 	
	 //�ڡ� �������  �ڡ�// 
	 
	 SOCKET mh_listen_socket;
	 mh_listen_socket = socket(AF_INET, SOCK_STREAM, 0); 
	 
	//socket(AF_INET, SOCK_STREAM, 0); 
	//AF_INET = IPv4 
	//SOCK_STREAM :TCP 
	// 0 : �������� ������
	 
	 //�ڡ� ��� ��  �ڡ�// 
	 
	 
	 
	 
	 //	�ڡ�cpp ���� �ڡ�//
	  
	
	//���� ���� 
	sockaddr_in srv_addr; 							
	srv_addr.sin_family = AF_INET;					
	srv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	//htons() OS ���� ����ϴ� ������ ��ȯ �ϴ°� �޶� ������ �߻��ϴ°� ����
	srv_addr.sin_port = htons(20001); 


	//�������� �Է� 
	bind(mh_listen_socket, (SOCKADDR*)&srv_addr, sizeof(srv_addr));
	
	//
	listen(mh_listen_socket, 1); //����ڸ� ������ �ִ� �� (1�� �ǹ�) ex) 

	//accept() Ŭ���̾�Ʈ �����ߴ��� �˼��ִ�. ��)Ŭ���̾�Ʈ ���Ӿ��ϸ� ����������� ����  =  ������(������ �̿�)/�񵿱����� ����)
	//mh_listen_socket ���Ͽ� ����ڰ� ���������� �߰��ϸ�(FD_ACCEPT)  m_hWnd�� 27001���� ����)
	WSAAsyncSelect(mh_listen_socket, m_hWnd, 27001, FD_ACCEPT);
	
	
	
	LRESULT CTestServerDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// wParam �ڵ� ��   ,  lParam : ����Ȯ��
	if (27001 == message) {
		if (MAX_CLIENT_COUNT > m_client_cout) {
			SOCKET h_socket = (SOCKET)wParam; //mh_listen_socket �ڵ鰪�� ����

			sockaddr_in client_addr;
			int sockaddr_in_size = sizeof(client_addr);

			mh_client_list[m_client_cout] = accept(h_socket, (SOCKADDR*)&client_addr, &sockaddr_in_size);
			WSAAsyncSelect(mh_client_list[m_client_cout], m_hWnd, 27002, FD_READ | FD_CLOSE);
			m_client_cout++;

			CString ip_address;
			ip_address = inet_ntoa(client_addr.sin_addr);

			MessageBox(ip_address, L"���ο� Ŭ���̾�Ʈ�� ������ �߽��ϴ�.:", MB_OK);
		}
		else {

		}
	}

	return CDialogEx::WindowProc(message, wParam, lParam);
}
	
	//	�ڡ�cpp ���� �� �ڡ�//
	
	
	
