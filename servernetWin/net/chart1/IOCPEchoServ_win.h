#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<process.h>
#include<WinSock2.h>

#define BUF_SIZE  100
#define READ   3
#define WRITE  5

typedef struct {
	SOCKET hClntSock;
	SOCKADDR_IN clntAdr;
} PER_HANDLE_DATA, * LPPER_HANDLE_DATA;

typedef struct {
	OVERLAPPED overlapped;
	WSABUF wsaBuf;
	char buffer[BUF_SIZE];
	int rwMode;
} PER_IO_DATA, * LPPER_IO_DATA;

unsigned WINAPI EchoThreadMain(LPVOID CompletionPortIO);
void ErrorHandling(const char* message);

//结构体变量地址与结构体第一个成员的地址值相同

int IOCP_Echo_Serve_win(char* port)
{
	HANDLE hComPort;
	SYSTEM_INFO sysInfo;
	LPPER_IO_DATA ioInfo;
	LPPER_HANDLE_DATA handleInfo;

	SOCKET hServSock;
	SOCKADDR_IN servAdr;
	int i;
	DWORD recvBytes,  flags = 0;

	hComPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	if (hComPort == NULL)
		ErrorHandling("CreateIoCompletionPort() error");
	else
		printf("hComport one is %d\n", hComPort);

	GetSystemInfo(&sysInfo);
	//根据cpu数量创建对应的线程
	printf("system cpu core number:%d\n", sysInfo.dwNumberOfProcessors);
	for ( i = 0; i < 1; i++)
	{
		_beginthreadex(NULL, 0, EchoThreadMain,(LPVOID)hComPort, 0, NULL);
	}

	//创建重叠IO的socket
	hServSock = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi(port));

	//服务器端  bind一个地址
	if (bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");
	if (listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");

	while (1)
	{
		SOCKET hClntSock;
		SOCKADDR_IN clntAdr;
		int addrLen = sizeof(clntAdr);

		hClntSock = accept(hServSock, (SOCKADDR*)&clntAdr, &addrLen);
		printf("accept() return socket:%d\n", hClntSock);

		//handleInfo主要是存放数据传输的socket
		handleInfo = (LPPER_HANDLE_DATA)malloc(sizeof(PER_HANDLE_DATA));
		handleInfo->hClntSock = hClntSock;
		memcpy(&(handleInfo->clntAdr), &clntAdr, addrLen);

		//只对数据传输的socket建立CP对象的连接
		hComPort = CreateIoCompletionPort((HANDLE)hClntSock, hComPort, (ULONG_PTR)handleInfo, 0);

		if (hComPort == NULL)
			ErrorHandling("CreateIoCompletionPort() error about link");
		else
			printf("hComport two is %d\n", hComPort);

		ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
		memset(ioInfo, 0, sizeof(PER_IO_DATA));

		ioInfo->wsaBuf.len = BUF_SIZE;
		ioInfo->wsaBuf.buf = ioInfo->buffer;
		ioInfo->rwMode = READ;
		WSARecv(handleInfo->hClntSock, &(ioInfo->wsaBuf), 1, &recvBytes, &flags, &(ioInfo->overlapped), NULL);
	}

	return 0;
}


/// <summary>
/// 多线程处理的函数，多线程处理对应的数据传输socket事件
/// </summary>
/// <param name="pComPort"></param>
/// <returns></returns>

unsigned WINAPI EchoThreadMain(LPVOID pComPort)
{
	puts("EchoThreadMain run.....");
	HANDLE hComPort = (HANDLE)pComPort;
	SOCKET sock;
	DWORD bytesTrans;
	LPPER_HANDLE_DATA handleInfo;
	LPPER_IO_DATA ioInfo ;
	DWORD flags = 0;

	while (1) {
		//INFINITE 一直阻塞到有IO事件完成
		puts("GetQueuedCompletionStatus before");

		if (!GetQueuedCompletionStatus(hComPort, &bytesTrans,
			(PULONG_PTR)&handleInfo, (LPOVERLAPPED*)&ioInfo, INFINITE)) {
			printf("GetQueuedCompletionStatus error:%d\n", WSAGetLastError());
		}

		puts("GetQueuedCompletionStatus finished");

		if (handleInfo != nullptr)
			ErrorHandling("handleInfo is not null ");

		sock = handleInfo->hClntSock;

		puts("data come....");

		if (ioInfo->rwMode == READ) {
			puts("message received!");
			if (bytesTrans == 0) //传输EOF时
			{
				closesocket(sock);
				free(handleInfo);
				free(ioInfo);
				continue;
			}

			memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
			ioInfo->wsaBuf.len = bytesTrans;
			ioInfo->rwMode = WRITE;
			WSASend(sock, &(ioInfo->wsaBuf), 1, NULL, 0, &(ioInfo->overlapped), NULL);

			ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
			memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
			ioInfo->wsaBuf.len = BUF_SIZE;
			ioInfo->wsaBuf.buf = ioInfo->buffer;
			ioInfo->rwMode = READ;
			WSARecv(sock, &(ioInfo->wsaBuf), 1, NULL, &flags, &(ioInfo->overlapped), NULL);
		}
		else
		{
			puts("message send!");
			free(ioInfo);
		}
	}

	return 0;
}












void ErrorHandling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}