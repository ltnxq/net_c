#pragma once

#include<cstdio>
#include<stdlib.h>
#include<WinSock2.h>
#include<string.h>




void error_handling(const char* message);

int get_sockType() {
	SOCKET tcp_sock, udp_sock;
	int sock_type;
	int optlen;
	int state;

	optlen = sizeof(sock_type);
	tcp_sock = socket(PF_INET, SOCK_STREAM, 0);
	udp_sock = socket(PF_INET, SOCK_DGRAM, 0);

	state = getsockopt(tcp_sock, SOL_SOCKET, SO_TYPE, (char*)&sock_type,&optlen);
	if (state)
	{
		error_handling("getsockopt error\n");
	}
	printf("Sock one type is %d:\n ", sock_type);

	state = getsockopt(udp_sock, SOL_SOCKET, SO_TYPE, (char*)&sock_type, &optlen);
	if (state)
	{
		error_handling("getsockopt error\n");
	}
	printf("Sock two type is %d:\n ", sock_type);

	return 0;
}

void error_handling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}