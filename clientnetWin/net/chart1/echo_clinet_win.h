#pragma once
#include<cstdio>
#include<stdlib.h>
#include<WinSock2.h>

#define BUF_SIZE 1024
void ErrorHandling(const char* message);


int echo_client_win(char* ip, char* port)
{
	WSADATA wsaData;
	SOCKET sock;
	SOCKADDR_IN serverAddr;
	int  str_len ,readLen;
	char message[BUF_SIZE] = { 0 };

	if (ip == nullptr || port == nullptr)
	{
		printf("ip or prot is null");
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.S_un.S_addr = inet_addr(ip);
	serverAddr.sin_port = htons(atoi(port));
	
	if (connect(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
		ErrorHandling("connect() error!");
	}

	while (1) {
		fputs("Insert message(q to quit): ", stdout);
		fgets(message, BUF_SIZE, stdin);
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

		//发送给服务端
		str_len = strlen(message);

		send(sock, message, str_len, 0);
		readLen = 0;
		while (1) 
		{
			readLen+= recv(sock, &message[readLen],BUF_SIZE-1, 0);
			if (readLen >= str_len)
				break;
		}
		message[readLen] = 0;
		printf("Message from server: %s", message);
	}

	closesocket(sock);
	return 0;
}

void ErrorHandling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}