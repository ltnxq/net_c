#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)

#include "chart1/echo_clinet_win.h"

int main(int argc, char* argv[]) {
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		return -1;
	}

	printf("main started \n");
	echo_client_win(argv[1],argv[2]);
	//sync_event();
	printf("main ended \n");


	WSACleanup();

	
	return 0;
}



