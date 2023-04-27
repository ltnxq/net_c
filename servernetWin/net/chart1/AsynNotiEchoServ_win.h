#pragma once
#include<stdio.h>
#include<string.h>
#include<winsock2.h>

#define BUF_SIZE 100

void CompressSockets(SOCKET hSockArr[], int idx, int total);
void CompressEvents(WSAEVENT hEventArr[], int idx, int total);
void ErrorHandling(const char* message);

int async_noti_echo_serv_win(char* port)
{
	SOCKET hServeSock, hClntSock;
	SOCKADDR_IN serAdr, clntAdr;

	//socket���ݡ��¼���������
	SOCKET hSockArr[WSA_MAXIMUM_WAIT_EVENTS];
	WSAEVENT hEventArr[WSA_MAXIMUM_WAIT_EVENTS];
	WSAEVENT newEvent;
	WSANETWORKEVENTS netEvents;

	int numofClntSock = 0;
	int strLen, i;
	int posInfo, startIdx;
	int clntAdrLen;
	char msg[BUF_SIZE];

	if (port == nullptr)
	{
		printf(" port is null");
		exit(1);
	}

	hServeSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServeSock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&serAdr, 0, sizeof(serAdr));
	serAdr.sin_family = AF_INET;
	serAdr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	serAdr.sin_port = htons(atoi(port));

	if (bind(hServeSock, (SOCKADDR*)&serAdr, sizeof(serAdr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");
	if (listen(hServeSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");

	//�����¼�����
	newEvent = WSACreateEvent();
	//����socket���¼�����֮�������
	if(WSAEventSelect(hServeSock,newEvent,FD_ACCEPT) == SOCKET_ERROR)
		ErrorHandling("WSAEventSelect() error");

	hSockArr[numofClntSock] = hServeSock;
	hEventArr[numofClntSock] = newEvent;
	numofClntSock++;

	while (1) {
		posInfo = WSAWaitForMultipleEvents(numofClntSock, hEventArr, false, WSA_INFINITE, FALSE);
		startIdx = posInfo - WSA_WAIT_EVENT_0;
		printf("startIdx is %d \n", startIdx);
		for (i = startIdx; i < numofClntSock; i++) {
			int sigEventIdx = WSAWaitForMultipleEvents(1, &hEventArr[i], TRUE, 0, FALSE);
			if ((sigEventIdx == WSA_WAIT_FAILED || sigEventIdx == WSA_WAIT_TIMEOUT)) {
				continue;
			}
			else {
				sigEventIdx = i;
				//�����¼�����
				WSAEnumNetworkEvents(hSockArr[sigEventIdx], hEventArr[sigEventIdx], &netEvents);
				if (netEvents.lNetworkEvents & FD_ACCEPT) {
					if (netEvents.iErrorCode[FD_ACCEPT_BIT] != 0) {
						puts("Accept Error");
						break;
					}

					clntAdrLen = sizeof(clntAdr);
					hClntSock = accept(hSockArr[sigEventIdx], (SOCKADDR*)&clntAdr, &clntAdrLen);
					newEvent = WSACreateEvent();
					WSAEventSelect(hClntSock, newEvent, FD_READ | FD_CLOSE);

					hSockArr[numofClntSock] = hClntSock;
					hEventArr[numofClntSock] = newEvent;
					numofClntSock++;
					puts("new client connected.....");
				}

				//����READ����
				if (netEvents.lNetworkEvents & FD_READ) {
					if (netEvents.iErrorCode[FD_READ_BIT] != 0) {
						puts("Read Error");
						break;
					}

					strLen = recv(hSockArr[sigEventIdx], msg, sizeof(msg), 0);
					send(hSockArr[sigEventIdx], msg, strLen, 0);
				}

				//����Ͽ�����
				if (netEvents.lNetworkEvents & FD_CLOSE) {
					if (netEvents.iErrorCode[FD_CLOSE_BIT] != 0) {
						printf("error code is %d \n", netEvents.iErrorCode[FD_CLOSE_BIT]);
						puts("CLOSE Error");
						break;
					}
					//�ر��¼����󡢹ر�socket
					WSACloseEvent(hEventArr[sigEventIdx]);
					closesocket(hSockArr[sigEventIdx]);

					numofClntSock--;
					//���������Ƴ���Ӧ��socket��event
					CompressSockets(hSockArr, sigEventIdx, numofClntSock);
					CompressEvents(hEventArr, sigEventIdx, numofClntSock);

					puts("client disconnected");
				}

			}
		}
	}
	return 0;
}


void CompressSockets(SOCKET hSockArr[], int idx, int total)
{
	int i;
	for (i = idx; i < total; i++)
		hSockArr[i] = hSockArr[i + 1];
}

void CompressEvents(WSAEVENT hEventArr[], int idx, int total)
{
	int i;
	for (i = idx; i < total; i++)
		hEventArr[i] = hEventArr[i + 1];
}



void ErrorHandling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}