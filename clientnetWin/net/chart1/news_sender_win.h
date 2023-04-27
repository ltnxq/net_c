#pragma once
#include<cstdio>
#include<stdlib.h>
#include<WinSock2.h>
#include<WS2tcpip.h>

#define BUF_SIZE 10
#define TTL 64
void ErrorHandling(const char* message);


int news_send_client_win(char* ip, char* port)
{
	WSADATA wsaData;
	SOCKET send_sock;
	SOCKADDR_IN multiAddr;
	int time_alive = TTL;
	int  str_len = 0;
	char buf[BUF_SIZE] = { 0 };
	FILE* fp;

	if (ip == nullptr || port == nullptr)
	{
		printf("ip or prot is null");
		exit(1);
	}

	if ((fp = fopen("news.txt", "r")) == nullptr) {
		ErrorHandling("fopen error");
	}


	send_sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (send_sock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&multiAddr, 0, sizeof(multiAddr));
	multiAddr.sin_family = AF_INET;
	multiAddr.sin_addr.S_un.S_addr = inet_addr(ip);
	multiAddr.sin_port = htons(atoi(port));

	//…Ë÷√TTL
	setsockopt(send_sock, IPPROTO_IP, IP_MULTICAST_TTL, (char*)&time_alive, sizeof(time_alive));

	while (!feof(fp))
	{
		fgets(buf, BUF_SIZE, fp);
		sendto(send_sock, buf, strlen(buf), 0, (struct sockaddr*)&multiAddr, sizeof(multiAddr));
		Sleep(5000);
	}


	closesocket(send_sock);

	return 0;


}

void ErrorHandling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}