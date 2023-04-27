
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4996)

#include "chart1/IOCPEchoServ_win.h"


int main(int argc, char* argv[])
{
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		return -1;
	}

	printf("main started \n");
	IOCP_Echo_Serve_win(argv[1]);
	printf("main ended \n");

	WSACleanup();


}
