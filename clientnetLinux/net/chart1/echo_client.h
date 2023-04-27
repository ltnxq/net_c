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
		
		//������������
		//tcp�������ޱ߽�ģ���������������ַ�����������һ���Խ��ܵĻ�����ô���ܵ��ķ������ݻ�������
		//tcp ���������ܵ�������̫����ô��ִ������ͣ���ô�ͻ�����û��ȫ�����յ���Ϣ������´�ӡ����Ϣ
		//��������д�����Խ���������ֶ�η�����Ϣ���ͻ���
		//�������Ϊ��������ԭ�����ؽ��յ����ַ��������Կͻ��˿���ͨ�����͵����ݳ���ȥ�жϣ�����ͻ��˲�֪���������˷��͵Ĺ�����ɶ
		//��ô����Ҫ����Ӧ�ò��Э�飬����http ��ͨ��һЩ����ı�־����ʾ��һ�ַ������ݵ���ֹ
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