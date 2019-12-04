#include <iostream>
#include <WS2tcpip.h>
#include <string.h>
#pragma comment (lib, "ws2_32.lib")

using namespace std;

void main(int argc, char* argv[])  // we can pass in a command line option!!
{
	// Startup Winsock
	WSADATA data;
	WORD version = MAKEWORD(2, 2);
	int wsok = WSAStartup(version, &data);
	if (wsok != 0)
	{
		cout << "Can't start Winsock!" << wsok;
	}

	WSADATA wsData;
	WSAStartup(MAKEWORD(2, 2), &wsData);


	// Bind socket to ip address and port
	SOCKET in = socket(AF_INET, SOCK_DGRAM, 0);
	SOCKADDR_IN serverHint;
	serverHint.sin_addr.S_un.S_addr = ADDR_ANY;
	serverHint.sin_family = AF_INET;
	serverHint.sin_port = htons(61234);

	if (bind(in, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR)
	{
		cout << "Can't bind socket!" << WSAGetLastError() << endl;
		return;
	}

	//create a hin structure for the server
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_port = htons(61234);
	inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

	//Socket creation
	SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);

	//Wirte out to to that socket
	string  s(argv[1]);
	int sendok = sendto(out, s.c_str(), s.size() + 1, 0, (sockaddr*)&server, sizeof(server));

	if (sendok == SOCKET_ERROR)
	{
		cout << "That didn't work!" << WSAGetLastError() << endl;
	}

	//close the socket
	closesocket(out);

	// Shutdown winsock
	WSACleanup();
}