#include <iostream>
#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")
using namespace std;

void main()
{
	// Startup Winsock
	WSADATA data;
	WORD version = MAKEWORD(2, 2);
	int wsok = WSAStartup(version, &data);
	if (wsok != 0){
		cout << "Can't start Winsock!" << wsok;
	}

	// Bind socket to ip address and port
	SOCKET in = socket(AF_INET, SOCK_DGRAM, 0);
	SOCKADDR_IN serverHint;
	serverHint.sin_addr.S_un.S_addr = ADDR_ANY;
	serverHint.sin_family = AF_INET;
	serverHint.sin_port = htons(61234);

	if (bind(in, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR){
		cout << "Can't bind socket!" << WSAGetLastError() << endl;
		return;
	}

	sockaddr_in client;
	int clientLength = sizeof(client);
	ZeroMemory(&client, sizeof(client));
	
	char buf[1024];
	ZeroMemory(buf, 1024);
	
	// Enter a loop
	while (true)
	{
		ZeroMemory(buf, 1024);

		// Wait for message
		int byesIn = recvfrom(in, buf, 1024 , 0,(sockaddr*)&client, &clientLength);
		if (byesIn == SOCKET_ERROR){
			cout << "Error receiving from client " << WSAGetLastError() << endl;
			continue;
		}

		// Display message and client info
		char clientIP[256];
		ZeroMemory(clientIP, 256);

		inet_ntop(AF_INET, &client.sin_addr, clientIP, 256);
		cout << "Message rect from " << clientIP << " :" << buf << endl;
	}
		
	// close socket
	closesocket(in);

	// Shutdown winsock
	WSACleanup();
}