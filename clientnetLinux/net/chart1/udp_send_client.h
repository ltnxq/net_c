#pragma once
#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

void error_handling(char* messae);

int udp_send_client(char* ip, char* port)
{

	if (ip == nullptr || port == nullptr) {
		printf("ip or port is null\n");
		exit(1);
	}

	int sock;
	char msg1[] = "hi!";
	char msg2[] = "i am another udp host";
	char msg3[] = "nice to meet you!";

	struct sockaddr_in serv_addr;


	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (sock == -1)
		error_handling("socke() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(ip);
	serv_addr.sin_port = htons(atoi(port));
    
	sendto(sock, msg1, sizeof(msg1), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	sendto(sock, msg2, sizeof(msg2), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	sendto(sock, msg3, sizeof(msg3), 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	close(sock);
	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

