#pragma once
#include<cstdio>
#include<stdlib.h>
#include<WinSock2.h>
#include<WS2tcpip.h>


#define BUF_SIZE 30
void ErrorHandling(const char* message);


int news_receive_win(char* multiIp,char* myIp,char* port)
{
	SOCKET hrecvSock;
	SOCKADDR_IN serverAddr;
	struct ip_mreq join_adr;
	int szClntAddr, str_len;
	char buf[BUF_SIZE] ;

	if ( multiIp == nullptr || myIp == nullptr ||port == nullptr)
	{
		printf("multiIp or myIp or port is null");
		exit(1);
	}

	hrecvSock = socket(PF_INET, SOCK_DGRAM, 0);
	if (hrecvSock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.S_un.S_addr = inet_addr(myIp);
	serverAddr.sin_port = htons(atoi(port));

	//服务器端  bind一个地址
	if (bind(hrecvSock, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

	//绑定广播地址和对应主机地址
	join_adr.imr_multiaddr.S_un.S_addr = inet_addr(multiIp);
	join_adr.imr_interface.S_un.S_addr = inet_addr(myIp);
	//设置sock选项，加入多播组
	setsockopt(hrecvSock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&join_adr, sizeof(join_adr));

	while (1)
	{
		str_len = recvfrom(hrecvSock, buf, BUF_SIZE - 1, 0, NULL, 0);
		if (str_len < 0)
			break;
		buf[str_len] = 0;
		fputs(buf, stdout);
	}


	closesocket(hrecvSock);

	WSACleanup();
	return 0;


}

void ErrorHandling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}