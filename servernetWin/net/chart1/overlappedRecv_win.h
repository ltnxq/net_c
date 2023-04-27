#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<WinSock2.h>

#define BUF_SIZE  1024
void ErrorHandling(const char* message);

int overlapped_recv_win(char* port)
{
	if (port == nullptr) {
		ErrorHandling("port is null");
	}

	SOCKET hLisnSock, hRecvSock;
	SOCKADDR_IN lisnAdr, recvAdr;
	int recvAdrSz;

	WSABUF dataBuf;
	WSAEVENT evObj;
	WSAOVERLAPPED overlapped;

	char buf[BUF_SIZE];
	DWORD recvBytes = 0, flags = 0;

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
	evObj = WSACreateEvent();
	memset(&overlapped, 0, sizeof(overlapped));
	overlapped.hEvent = evObj;
	dataBuf.len = BUF_SIZE;
	dataBuf.buf = buf;

	//这也是异步?
	if (WSARecv(hRecvSock, &dataBuf, 1, &recvBytes, &flags, &overlapped, NULL) == SOCKET_ERROR) {
		if (WSAGetLastError() == WSA_IO_PENDING) {
			puts("Backgroud data receive");
			WSAWaitForMultipleEvents(1, &evObj, TRUE, WSA_INFINITE, FALSE);
			WSAGetOverlappedResult(hRecvSock, &overlapped, &recvBytes, FALSE, NULL);
		}
		else {
			ErrorHandling("WSARecv() error");
		}
	}
	printf("Received message:%s\n", buf);
	WSACloseEvent(evObj);
	closesocket(hRecvSock);
	closesocket(hLisnSock);
	return 0;
}



















void ErrorHandling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}