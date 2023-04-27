#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<WinSock2.h>

#define BUF_SIZE 1024
void CALLBACK ReadCompRoutine(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
void CALLBACK WriteCompRoutine(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
void ErrorHandling(const char* message);

typedef struct {
	SOCKET hClntSock;
	char buf[BUF_SIZE];
	WSABUF wasBuf;
} PER_IO_DATA, * LPPER_IO_DATA;

int comRoutine_echo_serve_win(char* port)
{
	if (port == nullptr) {
		ErrorHandling("port is null");
	}

	SOCKET hLisnSock, hRecvSock;
	SOCKADDR_IN lisnAdr, recvAdr;
	int recvAdrSz;
	LPWSAOVERLAPPED lpOvlp;
	LPPER_IO_DATA hbInfo;
	DWORD recvBytes = 0;


	WSAEVENT evObj;
	WSAOVERLAPPED overlapped;

	DWORD  flags = 0;

	hLisnSock = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	u_long mode = 1;
	//设置为非阻塞的socket,很关键的一步
	ioctlsocket(hLisnSock, FIONBIO,&mode);

	memset(&lisnAdr, 0, sizeof(lisnAdr));
	lisnAdr.sin_family = AF_INET;
	lisnAdr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	lisnAdr.sin_port = htons(atoi(port));

	//服务器端  bind一个地址
	if (bind(hLisnSock, (SOCKADDR*)&lisnAdr, sizeof(lisnAdr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");
	if (listen(hLisnSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");

	recvAdrSz = sizeof(recvAdr);

	while (1)
	{
		//程序进入alertable wait 状态
		SleepEx(100, TRUE);
		hRecvSock = accept(hLisnSock, (SOCKADDR*)&recvAdr, &recvAdrSz);
		if (hRecvSock == INVALID_SOCKET) {
			if (WSAGetLastError() == WSAEWOULDBLOCK)
				continue;
			else
				ErrorHandling("accept() error");
		}
		puts("client connected.......");

		lpOvlp = (LPWSAOVERLAPPED)malloc(sizeof(WSAOVERLAPPED));
		memset(lpOvlp, 0, sizeof(WSAOVERLAPPED));

		hbInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
		memset(hbInfo, 0, sizeof(PER_IO_DATA));

		hbInfo->hClntSock = (DWORD)hRecvSock;
		(hbInfo->wasBuf).buf = hbInfo->buf;
		(hbInfo->wasBuf).len = BUF_SIZE;
		
		//基于Completion Routine函数的重叠IO中不需要事件对象，因为hEvent中可以写入其他信息
		lpOvlp->hEvent = (HANDLE)hbInfo;
		WSARecv(hRecvSock, &(hbInfo->wasBuf), 1, &recvBytes, &flags, lpOvlp, ReadCompRoutine);

	}

	closesocket(hRecvSock);
	closesocket(hLisnSock);
	return 0;
}

void CALLBACK ReadCompRoutine(DWORD dwError, DWORD szRecvBuyes, LPWSAOVERLAPPED lpOverlapped, DWORD flags)
{
	LPPER_IO_DATA hbInfo = (LPPER_IO_DATA)(lpOverlapped->hEvent);
	SOCKET hSock = hbInfo->hClntSock;
	LPWSABUF bufInfo = &(hbInfo->wasBuf);
	DWORD sentBytes;

	if (szRecvBuyes == 0) {//关闭socket
		closesocket(hSock);
		free(lpOverlapped->hEvent);
		free(lpOverlapped);
		puts("Client disconnected.........");
	}
	else {//echo
		bufInfo->len = szRecvBuyes;
		printf("receive message length:%d\n", szRecvBuyes);
		printf("send message to Client: %s\n", bufInfo->buf);
		WSASend(hSock, bufInfo, 1, &sentBytes, 0, lpOverlapped, WriteCompRoutine);
	}
}

//WSASend IO完成  进入REV状态 RECV事件发生  ReadCompRoutine
void CALLBACK WriteCompRoutine(DWORD dwError, DWORD szRecvBuyes, LPWSAOVERLAPPED lpOverlapped, DWORD flags)
{
	LPPER_IO_DATA hbInfo = (LPPER_IO_DATA)(lpOverlapped->hEvent);
	SOCKET hSock = hbInfo->hClntSock;
	LPWSABUF bufInfo = &(hbInfo->wasBuf);
	DWORD recvBytes;
	DWORD  flagInfo = 0;

	WSARecv(hSock, bufInfo, 1, &recvBytes, &flagInfo, lpOverlapped, ReadCompRoutine);
}


void ErrorHandling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}






