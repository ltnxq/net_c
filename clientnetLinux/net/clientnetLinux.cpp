#include "chart1/echo_client.h"


int main(int argc,char *argv[]) {
	printf("main started\n");
	echo_client(argv[1], argv[2]);
	printf("main end\n");
}