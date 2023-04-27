#pragma once
#include<cstdio>
#include<stdlib.h>
#include<WinSock2.h>

#define BUF_SIZE 30
void ErrorHandling(const char* message);


int udp_echo_server_win(char* port)
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN serverAddr, clntAddr;
	int szClntAddr, str_len;
	char message[] = "Hello World";

	if (port == nullptr)
	{
		printf("prot is null");
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hServSock = socket(PF_INET, SOCK_DGRAM, 0);
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(atoi(port));

	//服务器端  bind一个地址
	if (bind(hServSock, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");


	while (true)
	{
		szClntAddr = sizeof(clntAddr);
		str_len = recvfrom(hServSock, message, BUF_SIZE, 0, (SOCKADDR*)&clntAddr, &szClntAddr);
		sendto(hServSock, message, str_len, 0, (SOCKADDR*)&clntAddr, sizeof(clntAddr));
	}

	
	closesocket(hServSock);

	WSACleanup();
	return 0;


}

void ErrorHandling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}