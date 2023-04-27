#pragma once

#include<windows.h>
#include<stdio.h>
#include<process.h>

#define STR_LEN 100

unsigned WINAPI NumberOfA(void* arg);
unsigned WINAPI NumberOfOthers(void* arg);

static char str[STR_LEN];
static HANDLE event;

int sync_event()
{
	HANDLE hthread1, hthread2;

	//TRUE->MANUAL false->non-signaled
	event = CreateEvent(NULL, true, false, NULL);

	hthread1 = (HANDLE)_beginthreadex(NULL, 0, NumberOfA, NULL, 0, NULL);
	hthread2 = (HANDLE)_beginthreadex(NULL, 0, NumberOfOthers, NULL, 0, NULL);

	fputs("Input string: ", stdout);
	fgets(str, STR_LEN, stdin);
	//等待用户输入完成的话，那么可以让线程进行计算
	SetEvent(event);

	WaitForSingleObject(hthread1, INFINITE);
	WaitForSingleObject(hthread2, INFINITE);

	ResetEvent(event);
	CloseHandle(event);

	return 0;

}

unsigned WINAPI NumberOfA(void* arg) {
	int i, cnt = 0;
	WaitForSingleObject(event, INFINITE);
	for ( i = 0; str[i] != 0; i++)
	{
		if (str[i] == 'A')
			cnt++;
	}

	printf("Num of A:%d \n", cnt);
	return 0;

}

unsigned WINAPI NumberOfOthers(void* arg) {
	int i, cnt = 0;
	WaitForSingleObject(event, INFINITE);
	for (i = 0; str[i] != 0; i++)
	{   
		//最后是一个换行
		printf("NumberOfOthers:%d\n", str[i]);
		if (str[i] != 'A')
			cnt++;
	}

	//为啥减去1，因为最后一个是换行符
	printf("Num of others:%d \n", cnt-1);
	return 0;
}