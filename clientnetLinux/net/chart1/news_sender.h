#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define TTL 64
#define BUF_SIZE 30
void error_handling(char* messae);

int news_send_client(char* ip, char* port)
{

	if (ip == nullptr || port == nullptr) {
		printf("ip or port is null\n");
		exit(1);
	}

	int sock;
	int time_live = TTL;
	FILE* fp;
	char buf[BUF_SIZE];

	struct sockaddr_in mul_addr;
	if ((fp = fopen("news.txt", "r")) == nullptr)
		error_handling("fopen() error");

	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (sock == -1)
		error_handling("socke() error");

	

	memset(&mul_addr, 0, sizeof(mul_addr));
	mul_addr.sin_family = AF_INET;
	mul_addr.sin_addr.s_addr = inet_addr(ip);  //mulicate IP
	mul_addr.sin_port = htons(atoi(port));  //muliticate port

	//设置TTL
	//为啥需要设置TTL，由于经过一个路由TTL-1  当TTL=0的时候这个网络数据包就会被丢弃，所以尽可能把这个值设置的大一点
	setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&time_live, sizeof(time_live));
	
	while(!feof(fp))
	{
		fgets(buf, BUF_SIZE, fp);
		sendto(sock, buf, strlen(buf), 0, (struct sockaddr*)&mul_addr, sizeof(mul_addr));
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

