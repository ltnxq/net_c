#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>


void error_handling(char* message);

#define BUF_SIZE  1024

int file_client(char* ip,char* port)
{

	if (ip == nullptr || port == nullptr ) {
		printf("ip or port is null\n");
		exit(1);
	}

	int sock;
	int read_cnt;

	FILE* fp;

	struct sockaddr_in serv_addr;
	char buf[BUF_SIZE];

	fp = fopen("receive.dat", "wb");
	if (!fp) {
		printf("open file fail\n");
		exit(1);
	}

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socke() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(ip);
	serv_addr.sin_port = htons(atoi(port));

	connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	while ((read_cnt = read(sock, buf, BUF_SIZE)) != 0)
		fwrite((void*)buf, 1, read_cnt, fp);

	puts("Received file data\n");
	write(sock, "Tank you", 10);

	fclose(fp);
	close(sock);

	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}