#include<iostream>
#include "socketAPI.h"


using namespace std;

int main() {
	WSADATA wsdata;
	WSAStartup(0x2020, &wsdata);

	SOCKET hSocket;
	hSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);


	SOCKADDR_IN tAddr = {};
	tAddr.sin_family = PF_INET;
	tAddr.sin_port = htons(PORT);
	tAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

	connect(hSocket, (SOCKADDR*)&tAddr, sizeof(tAddr));

	char Messageg[] = "Client";
	send(hSocket, Messageg, strlen(Messageg), 0);

	char cBuffer[PACKET_SIZE] = {};
	recv(hSocket, cBuffer, PACKET_SIZE, 0);

	cout << "Recv Message :" << cBuffer << "\n";

	closesocket(hSocket);

	WSACleanup();
	return 0;
}