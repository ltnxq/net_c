#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<WinSock2.h>

#define BUF_SIZE  1024
//回调函数
void CALLBACK CompRoutine(DWORD, DWORD, LPWSAOVERLAPPED, DWORD);
void ErrorHandling(const char* message);

DWORD recvBytes = 0;
char buf[BUF_SIZE];
WSABUF dataBuf;

int comRoutine_recv_win(char* port)
{
	if (port == nullptr) {
		ErrorHandling("port is null");
	}

	SOCKET hLisnSock, hRecvSock;
	SOCKADDR_IN lisnAdr, recvAdr;
	int recvAdrSz;

	
	WSAEVENT evObj;
	WSAOVERLAPPED overlapped;

	DWORD  flags = 0;

	hLisnSock = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
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
	hRecvSock = accept(hLisnSock, (SOCKADDR*)&recvAdr, &recvAdrSz);

	memset(&overlapped, 0, sizeof(overlapped));
	dataBuf.len = BUF_SIZE;
	dataBuf.buf = buf;
	evObj = WSACreateEvent();
	

	//传递的回调函数指针
	if (WSARecv(hRecvSock, &dataBuf, 1, &recvBytes, &flags, &overlapped, CompRoutine) == SOCKET_ERROR) {
		if (WSAGetLastError() == WSA_IO_PENDING) 
			puts("Backgroud data receive");
	}

	int idx = WSAWaitForMultipleEvents(1, &evObj, false, WSA_INFINITE, TRUE);
	if (idx == WAIT_IO_COMPLETION)
		puts("Overlapped IO completed");
	else
		ErrorHandling("WSARecv() error");

	WSACloseEvent(evObj);
	closesocket(hRecvSock);
	closesocket(hLisnSock);
	return 0;
}

void CALLBACK CompRoutine(DWORD dwError, DWORD szRecvBuyes, LPWSAOVERLAPPED lpOverlapped, DWORD flags)
{
	if (dwError != 0) {
		ErrorHandling("ComRoutine error");
	}
	else {
		recvBytes = szRecvBuyes;
		printf("Received msglen-%d  message:%s\n",recvBytes, buf);
	}
}




void ErrorHandling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}