#pragma once

#include<cstdio>
#include<stdlib.h>
#include<WinSock2.h>
#include<string.h>
#include<WS2tcpip.h>




void error_handling(const char* message);
//Severity	Code	Description	Project	File	Line	Suppression State
//Error	C4996	'inet_addr': Use inet_pton() or InetPton() instead or define _WINSOCK_DEPRECATED_NO_WARNINGS to disable deprecated API warnings	clientwinnet	

int hello_client_win(char* ip,char* port) {
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN serverAddr;

	char message[30];

	int strlen = 0;
	int idx = 0, readlen = 0;
	if (ip == nullptr || port == nullptr) {
		printf("ip or port is null");
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		error_handling("WSAStartup() error!");

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
		error_handling("socket() error");

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.S_un.S_addr = inet_addr(ip);
	serverAddr.sin_port = htons(atoi(port));

	if (connect(hSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		error_handling("connect() error!");
	}
	//ÎÞ±ß½çµÄ 
	while (readlen = recv(hSocket, &message[idx++], 1, 0)) {
		if (readlen == -1)
			error_handling("recv() error!");

		strlen += readlen;
	}

	printf("Message from server: %s \n", message);
	printf("Function read call count£º %d\n", strlen);

	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void error_handling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
