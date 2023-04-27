#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<Windows.h>
#include<process.h>

#define BUF_SIZE 100
#define NAME_SIZE 20

unsigned WINAPI send_msg(void* arg);
unsigned WINAPI recv_msg(void* arg);
void ErrorHandling(const char* message);

char name[NAME_SIZE] = "[DEFAULT]";
char msg[BUF_SIZE];

int chat_clnt_win(char* ip, char* port,const char* name)
{
	 SOCKET sock;
     SOCKADDR_IN serv_addr;
	 HANDLE send_thread, recv_thread;
	 void* thread_return;

	if (ip == nullptr || port == nullptr) {
		printf("ip OR port is null\n");
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(ip);
	serv_addr.sin_port = htons(atoi(port));

	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		ErrorHandling("connect() error");
	else
		puts("Connected.........");

	//分别创建读写线程用于读取和发送数据
	send_thread = (HANDLE)_beginthreadex(NULL, 0, send_msg, (void*)&sock, 0, NULL);
	recv_thread = (HANDLE)_beginthreadex(NULL, 0, recv_msg, (void*)&sock, 0, NULL);

	//等待所有线程结束
	WaitForSingleObject(send_thread, INFINITE);
	WaitForSingleObject(recv_thread, INFINITE);

	closesocket(sock);
	return 0;
}

unsigned WINAPI send_msg(void* arg)
{
	SOCKET sock = *((SOCKET*)arg);
	char name_msg[NAME_SIZE + BUF_SIZE];

	while (true)
	{
		fgets(msg, BUF_SIZE, stdin);
		if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n"))
		{
			closesocket(sock);
			exit(0);
		}
		sprintf(name_msg, "%s %s", name, msg);
		send(sock,name_msg,strlen(name_msg),0);
	}

	return 0;
}

unsigned WINAPI recv_msg(void* arg)
{
	SOCKET sock = *((SOCKET*)arg);
	char name_msg[NAME_SIZE + BUF_SIZE];
	int str_len;
	while (1)
	{
		str_len = recv(sock, name_msg, NAME_SIZE + BUF_SIZE - 1,0);
		if (str_len == -1)
			return -1;
		name_msg[str_len] = 0;
		fputs(name_msg, stdout);
	}

	return 0;
}

void ErrorHandling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}