#pragma once


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUF_SIZE 1024


void error_handling(char* message);

int sep_client(char* ipAddrss, char* port)
{
	int sock;
	struct sockaddr_in serv_addr;
	char buf[BUF_SIZE];
	int str_len;

	FILE* readfp;
	FILE* writefp;


	if (ipAddrss == nullptr || port == nullptr) {
		printf("ip OR port is null\n");
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(ipAddrss);
	serv_addr.sin_port = htons(atoi(port));

	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
		error_handling("connect() error");

	//将sock转换为文件描述符
	readfp = fdopen(sock, "r");
	writefp = fdopen(sock, "w");

	while (1)
	{
		if (fgets(buf, sizeof(buf), readfp) == NULL)
			break;
		fputs(buf, stdout);
		fflush(stdout);
	}

	//这个时候已经服务器端已经关闭了writefp了
	fputs("From client 3q \n", writefp);
	fflush(writefp);
	fclose(writefp); fclose(readfp);

	close(sock);
	return 0;

}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}