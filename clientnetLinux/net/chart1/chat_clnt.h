#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/time.h>
#include<sys/epoll.h>
#include<fcntl.h>
#include<errno.h>
#include<pthread.h>

#define BUF_SIZE 100
#define NAME_SIZE 20

void* send_msg(void* arg);
void* recv_msg(void* arg);
void error_handling(char* message);

char name[NAME_SIZE] = "[DEFAULT]";
char msg[BUF_SIZE];

int chat_clnt(char* ip, char* port,char* name)
{
	int sock;
	struct sockaddr_in serv_addr;
	pthread_t send_thread, recv_thread;
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
		error_handling("connect() error");
	else
		puts("Connected.........");

	//分别创建读写线程用于读取和发送数据
	pthread_create(&send_thread, NULL, send_msg, (void*)&sock);
	pthread_create(&recv_thread, NULL, recv_msg, (void*)&sock);
	pthread_join(send_thread, &thread_return);
	pthread_join(recv_thread, &thread_return);

	close(sock);
	return 0;
}

void* send_msg(void* arg)
{
	int sock = *((int*)arg);
	char name_msg[NAME_SIZE + BUF_SIZE];

	while (true)
	{
		fgets(msg, BUF_SIZE, stdin);
		if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n"))
		{
			close(sock);
			exit(0);
		}
		sprintf(name_msg, "%s %s", name, msg);
		write(sock, name_msg, strlen(name_msg));

	}

	return NULL;
}

void* recv_msg(void* arg)
{
	int sock = *((int*)arg);
	char name_msg[NAME_SIZE + BUF_SIZE];
	int str_len;
	while (1)
	{
		str_len = read(sock, name_msg, NAME_SIZE + BUF_SIZE - 1);
		if (str_len == -1)
			return (void*)-1;
		name_msg[str_len] = 0;
		fputs(name_msg, stdout);
	}

	return NULL;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}