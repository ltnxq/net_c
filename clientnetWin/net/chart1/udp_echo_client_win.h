#pragma once
#include<cstdio>
#include<stdlib.h>
#include<WinSock2.h>

#define BUF_SIZE 30
void ErrorHandling(const char* message);


int udp_echo_client_win(char* ip,char* port)
{
	WSADATA wsaData;
	SOCKET sock;
	SOCKADDR_IN serverAddr;
	int  str_len = 0;
	char message[BUF_SIZE] = {0};

	if (ip== nullptr || port == nullptr)
	{
		printf("ip or prot is null");
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (sock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.S_un.S_addr = inet_addr(ip);
	serverAddr.sin_port = htons(atoi(port));
	//调用connect 成为绑定的udp socket
	//后面直接可以调用send 或者  read函数
	connect(sock, (SOCKADDR*)&serverAddr, sizeof(serverAddr));

	while (1) {
		fputs("Insert message(q to quit): ", stdout);
		fgets(message, sizeof(message), stdin);
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;
		
		//发送给服务端
		send(sock, message, strlen( message), 0);
		str_len = recv(sock, message,sizeof(message)-1, 0);
		message[str_len] = 0;
		printf("Message from server: %s", message);
	}


	closesocket(sock);

	WSACleanup();
	return 0;


}

void ErrorHandling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}