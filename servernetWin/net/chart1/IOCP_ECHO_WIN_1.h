#pragma once

#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#define MAX_BUFF_SIZE 1024

// �����ص��ṹ��
typedef struct _PER_IO_OPERATION_DATA {
    WSAOVERLAPPED Overlapped;
    WSABUF DataBuf;
    CHAR Buffer[MAX_BUFF_SIZE];
    DWORD BytesSend;
    DWORD BytesRecv;
} PER_IO_OPERATION_DATA, * LPPER_IO_OPERATION_DATA;

int IOCP_WIN_2() {
    int iResult = 0;

    // ����socket�����ͻ�������
    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSocket == INVALID_SOCKET) {
        std::cout << "socket failed: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(8569);

    // �󶨵�ַ�Ͷ˿�
    iResult = bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (iResult == SOCKET_ERROR) {
        std::cout << "bind failed: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    // ��ʼ����
    iResult = listen(listenSocket, SOMAXCONN);
    if (iResult == SOCKET_ERROR) {
        std::cout << "listen failed: " << WSAGetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    // ����IOCP���
    HANDLE iocpHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    if (iocpHandle == NULL) {
        std::cout << "CreateIoCompletionPort failed: " << GetLastError() << std::endl;
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    // ������socket������IOCP��
    if (CreateIoCompletionPort((HANDLE)listenSocket, iocpHandle, 0, 0) == NULL) {
        std::cout << "CreateIoCompletionPort failed: " << GetLastError() << std::endl;
        CloseHandle(iocpHandle);
        closesocket(listenSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server started." << std::endl;

    while (true) {
        sockaddr_in clientAddr;
        int addrLen = sizeof(clientAddr);

        // ���տͻ�������
        SOCKET clientSocket = accept(listenSocket, (sockaddr*)&clientAddr, &addrLen);
        if (clientSocket == INVALID_SOCKET) {
            std::cout << "accept failed: " << WSAGetLastError() << std::endl;
            continue;
        }

        // ���ͻ���socket������IOCP��
        HANDLE clientHandle = CreateIoCompletionPort((HANDLE)clientSocket, iocpHandle, 0, 0);
        if (clientHandle == NULL) {
            std::cout << "CreateIoCompletionPort failed: " << GetLastError() << std::endl;
            closesocket(clientSocket);
            continue;
        }

        // �����ص��ṹ��
        LPPER_IO_OPERATION_DATA perIoData = new PER_IO_OPERATION_DATA();
        memset(&(perIoData->Overlapped), 0, sizeof(WSAOVERLAPPED));
        perIoData->DataBuf.len = MAX_BUFF_SIZE;
        perIoData->DataBuf.buf = perIoData->Buffer;

        DWORD bytesRecv = 0;
        DWORD flags = 0;

        // ���տͻ�������
        if (WSARecv(clientSocket, &(perIoData->DataBuf), 1, &bytesRecv, &flags, &(perIoData->Overlapped), NULL) == SOCKET_ERROR) {
            if (WSAGetLastError() != WSA_IO_PENDING) {
                std::cout << "WSARecv failed: " << WSAGetLastError() << std::endl;
                delete perIoData;
                closesocket(clientSocket);
                continue;
            }
        }

        std::cout << "Client connected: " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;
    }

    CloseHandle(iocpHandle);
    closesocket(listenSocket);
    WSACleanup();

    return 0;
}
