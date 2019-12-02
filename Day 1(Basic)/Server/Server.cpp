#include<iostream>
#include "socketAPI.h"


using namespace std;

int main() {
	WSADATA wsadata;
	WSAStartup(0x2020, &wsadata);

	SOCKET hListen;
	hListen = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	SOCKADDR_IN tListenAddr = {};
	tListenAddr.sin_family = PF_INET;
	tListenAddr.sin_port = htons(PORT);
	tListenAddr.sin_addr.s_addr = htonl(INADDR_ANY);

	bind(hListen, (SOCKADDR*)&tListenAddr, sizeof(tListenAddr));
	listen(hListen, SOMAXCONN);

	SOCKADDR_IN tClntAddr = {};
	int iClntSize = sizeof(tClntAddr);
	SOCKET hClient = accept(hListen, (SOCKADDR*)&tClntAddr, &iClntSize);

	char cBuffer[PACKET_SIZE] = {};
	recv(hClient, cBuffer, PACKET_SIZE, 0);
	cout << "Recv Message :" << cBuffer << "\n";

	char Messageg[] = "Server";
	send(hClient, Messageg, strlen(Messageg), 0);

	closesocket(hClient);
	closesocket(hListen);

	WSACleanup();
	return 0;
}
