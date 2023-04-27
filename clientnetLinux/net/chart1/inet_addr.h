#include<cstdio>
#include<arpa/inet.h>

int test_inet_aton() {
	char* addr1 = "127.232.124.79";
	struct sockaddr_in addr_inet;
	if (!inet_aton(addr1, &addr_inet.sin_addr))
		printf("error occur");
	else
		printf("addr:%#x\n", addr_inet.sin_addr.s_addr);
	
	return 0;

}