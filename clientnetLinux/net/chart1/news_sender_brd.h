#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUF_SIZE 30
void error_handling(char* messae);

int news_send_brd_client(char* ip, char* port)
{

	if (ip == nullptr || port == nullptr) {
		printf("ip or port is null\n");
		exit(1);
	}

	int sock;
	int so_brd = 1;
	FILE* fp;
	char buf[BUF_SIZE];

	struct sockaddr_in broad_addr;
	if ((fp = fopen("news.txt", "r")) == nullptr)
		error_handling("fopen() error");

	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (sock == -1)
		error_handling("socke() error");


	memset(&broad_addr, 0, sizeof(broad_addr));
	broad_addr.sin_family = AF_INET;
	broad_addr.sin_addr.s_addr = inet_addr(ip);  //broad_cast IP
	broad_addr.sin_port = htons(atoi(port));  //broad_cast port

	//socket开启广播,默认是不开启的

	int ret = setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void*)&so_brd, sizeof(so_brd));
	if (ret == -1)
		error_handling("setsockopt() error");

	while (!feof(fp))
	{
		fgets(buf, BUF_SIZE, fp);
		sendto(sock, buf, strlen(buf), 0, (struct sockaddr*)&broad_addr, sizeof(broad_addr));
		sleep(2);
	}

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

