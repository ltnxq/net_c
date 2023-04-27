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

//�ṹ�������ַ��ṹ���һ����Ա�ĵ�ֵַ��ͬ

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
	//����cpu����������Ӧ���߳�
	printf("system cpu core number:%d\n", sysInfo.dwNumberOfProcessors);
	for ( i = 0; i < 1; i++)
	{
		_beginthreadex(NULL, 0, EchoThreadMain,(LPVOID)hComPort, 0, NULL);
	}

	//�����ص�IO��socket
	hServSock = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	servAdr.sin_port = htons(atoi(port));

	//��������  bindһ����ַ
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

		//handleInfo��Ҫ�Ǵ�����ݴ����socket
		handleInfo = (LPPER_HANDLE_DATA)malloc(sizeof(PER_HANDLE_DATA));
		handleInfo->hClntSock = hClntSock;
		memcpy(&(handleInfo->clntAdr), &clntAdr, addrLen);

		//ֻ�����ݴ����socket����CP���������
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
/// ���̴߳���ĺ��������̴߳����Ӧ�����ݴ���socket�¼�
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
		//INFINITE һֱ��������IO�¼����
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
			if (bytesTrans == 0) //����EOFʱ
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