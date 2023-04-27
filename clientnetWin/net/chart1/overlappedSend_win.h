
#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<WinSock2.h>

void ErrorHandling(const char* message);

int overlapped_send_server_win(char* ip,char* port)
{
	if (ip == nullptr || port == nullptr)
		ErrorHandling("ip or port is null");
	
	SOCKET hSocket;
	SOCKADDR_IN sendAdr;

	WSABUF dataBuf;
	char msg[] = "Network is Computer!";
	DWORD sendBytes = 0;

	WSAEVENT evObj;
	WSAOVERLAPPED overlapped;

	//创建重叠IO的socket
	hSocket = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	memset(&sendAdr, 0, sizeof(sendAdr));
	sendAdr.sin_family = AF_INET;
	sendAdr.sin_addr.S_un.S_addr = inet_addr(ip);
	sendAdr.sin_port = htons(atoi(port));

	if (connect(hSocket, (SOCKADDR*)&sendAdr, sizeof(sendAdr)) == SOCKET_ERROR) {
		ErrorHandling("connect() error!");
	}

	evObj = WSACreateEvent();
	memset(&overlapped, 0, sizeof(overlapped));
	overlapped.hEvent = evObj;
	dataBuf.len = strlen(msg) + 1;
	dataBuf.buf = msg;

	if (WSASend(hSocket, &dataBuf, 1, &sendBytes, 0, &overlapped, NULL) == SOCKET_ERROR)
	{   
		//如果是pending状态，那么表示数据没有发送完成
		if (WSAGetLastError() == WSA_IO_PENDING) {
			puts("Background data send");
			WSAWaitForMultipleEvents(1, &evObj, TRUE, WSA_INFINITE, FALSE);
			WSAGetOverlappedResult(hSocket, &overlapped, &sendBytes, FALSE, NULL);
		}
		else {
			ErrorHandling("WSASend() error");
		}
	}

	printf("Send data size: %d \n", sendBytes);
	WSACloseEvent(evObj);
	closesocket(hSocket);
	return 0;

}




void ErrorHandling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

