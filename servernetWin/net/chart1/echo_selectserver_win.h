#pragma once
#include<cstdio>
#include<stdlib.h>
#include<WinSock2.h>

#define BUF_SIZE 30
void ErrorHandling(const char* message);


int echo_select_server_win(char* port)
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN serverAddr, clntAddr;
	int szClntAddr, str_len;
	char buf[BUF_SIZE];
	TIMEVAL timeout;

	fd_set reads, cpy_reads;

	if (port == nullptr)
	{
		printf("prot is null");
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(atoi(port));

	//服务器端  bind一个地址
	if (bind(hServSock, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");
	if(listen(hServSock,5) == SOCKET_ERROR)
		ErrorHandling("listen() error");
		
	FD_ZERO(&reads);
	FD_SET(hServSock, &reads);

	while (1) {
		cpy_reads = reads;
		timeout.tv_sec = 5;
		timeout.tv_usec = 2000;

		int fdNum = select(0, &cpy_reads, 0, 0,NULL);
		puts("return from select");
		if (fdNum == SOCKET_ERROR)
			break;
		if (fdNum == 0) {
			//Time-out
			puts("timeout");
			continue;
		} 

		szClntAddr = sizeof(clntAddr);
		for (int i = 0; i < reads.fd_count; i++) {
			int activeSocketFd = reads.fd_array[i];
			if (FD_ISSET(activeSocketFd, &cpy_reads)) {
				//服务端，说明有新的连接进来
				if (activeSocketFd == hServSock) {
					
					hClntSock= accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);
					//加入到select的监听队列中
					FD_SET(hClntSock, &reads);
					
					printf("Connected client is: %d\n ", hClntSock);
				}
				else {
					//read message,说明有数据请求过来，除了server_sock  其他都是对应的clnt_sock
					str_len = recv(activeSocketFd, buf, BUF_SIZE-1,0);
					if (str_len == 0) {//说明对方已经关闭
						FD_CLR(activeSocketFd, &reads);
						closesocket(activeSocketFd);
						printf("closed client : %d\n", activeSocketFd);
					}
					else {
						send(activeSocketFd, buf, str_len,0);  //echo!
					}
				}
			}
		}
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