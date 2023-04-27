#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>


void error_handling(char* message);

#define BUF_SIZE  1024


int echo_client(char* ipAddrss, char* port)
{
	int sock;
	struct sockaddr_in serv_addr;
	char message[BUF_SIZE];
	int str_len, recv_len, recv_cnt;

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
	else
		puts("Connected.........");
	while (1) {
		fputs("Input message(Q to quit): ", stdout);
		fgets(message, BUF_SIZE, stdin);

		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;
		
		//存在两个问题
		//tcp数据是无边界的，如果连续发两个字符串，服务器一次性接受的话，那么接受到的返回数据会有问题
		//tcp 服务器接受到的数据太大，那么会分次数发送，那么客户端在没有全部接收到信息的情况下打印了消息
		//下面这种写法可以解决服务器分多次发送消息给客户端
		//这个是因为服务器端原样返回接收到的字符串，所以客户端可以通过发送的数据长度去判断，加入客户端不知道服务器端发送的规则是啥
		//那么就需要定义应用层的协议，例如http ，通过一些特殊的标志来表示这一轮发送数据的终止
		str_len = write(sock, message, strlen(message));
		recv_len = 0;
		while (recv_len < str_len) {
			recv_cnt = read(sock, message, BUF_SIZE - 1);
			recv_len += recv_cnt;
		}
		message[recv_len] = 0;
		printf("Message from server: %s", message);
	}
	
	close(sock);
	return 0;

}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}