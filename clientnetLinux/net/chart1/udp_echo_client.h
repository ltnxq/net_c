#pragma once

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define BUF_SIZE 30
void error_handling(char* messae);

int udp_echo_client(char* ip,char* port)
{

	if (ip==nullptr || port == nullptr) {
		printf("ip or port is null\n");
		exit(1);
	}

	int sock;
	int str_len = 0;

	struct sockaddr_in serv_addr, from_adr;

	socklen_t adr_sz;

	char message[BUF_SIZE];

	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (sock == -1)
		error_handling("socke() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(ip);
	serv_addr.sin_port = htons(atoi(port));

	//


	while (1) {
		fputs("Insert message(q to quit): ", stdout);
		fgets(message, sizeof(message), stdin);
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

		sendto(sock, message, sizeof message, 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
		adr_sz = sizeof(from_adr);
		str_len = recvfrom(sock, message, BUF_SIZE, 0, (struct sockaddr*)&serv_addr, &adr_sz);
		message[str_len] = 0;
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

