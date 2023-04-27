#pragma once
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<netdb.h>

void error_handling(char* message);

int getIpFromDns(char* dName) {
	if (dName == nullptr)
	{
		printf("dName is null\n");
		return - 1;
	}

	int i = 0;
	struct hostent* host;

	host = gethostbyname(dName);
	if (!host)
	{
		error_handling("gethost.....error");
		return -1;
	}

	printf("Official name: %s \n", host->h_name);
	for (i = 0; host->h_aliases[i]; i++)
		printf("Aliases %d: %s \n", i + 1, host->h_aliases[i]);
	printf("Address type: %s \n",(host->h_addrtype == AF_INET) ? "AF_INET":"AF_INET6");
	for (i = 0; host->h_addr_list[i]; i++)
		printf("IP address %d:%s \n", i + 1, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));

	return 0;
}

int gethostByaddr(char* ip)
{
	int i;
	struct hostent* host;
	struct sockaddr_in addr;

	if (ip == nullptr) {
		printf("ip is null\n");
		return -1;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sin_addr.s_addr = inet_addr(ip);
	host = gethostbyaddr(static_cast<void*>(&addr.sin_addr), 4, AF_INET);
	if (!host) {
		printf("gethostbyaddr error\n");
		return -1;
	}
	printf("Official name: %s \n", host->h_name);
	for (i = 0; host->h_aliases[i]; i++)
		printf("Aliases %d: %s \n", i + 1, host->h_aliases[i]);
	printf("Address type: %s \n", (host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6");
	for (i = 0; host->h_addr_list[i]; i++)
		printf("IP address %d:%s \n", i + 1, inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));

	return 0;
}





void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}