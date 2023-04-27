#pragma once

#include<windows.h>
#include<stdio.h>
#include<process.h>

#define NUM_THREAD 50
unsigned WINAPI threadInc(void* arg);
unsigned WINAPI threadDes(void* arg);

long long num = 0;

int thread_2()
{
	HANDLE handles[NUM_THREAD];
	int i;
	printf("sizeof long long:%d\n",num);
	for (size_t i = 0; i < NUM_THREAD; i++)
	{
		if(i%2)
			handles[i] = (HANDLE)_beginthreadex(NULL, 0, threadInc,NULL, 0, NULL);
		else
			handles[i] = (HANDLE)_beginthreadex(NULL, 0, threadDes, NULL, 0, NULL);
	}

	WaitForMultipleObjects(NUM_THREAD, handles, true, INFINITE);
	printf("result:%lld \n", num);
	return 0;
}

unsigned WINAPI threadInc(void* arg)
{
	int i;
	for (i = 0; i < 50000000; i++)
		num += 1;
	return 0;
}

unsigned WINAPI threadDes(void* arg)
{
	int i;
	for (i = 0; i < 50000000; i++)
		num -= 1;
	return 0;
}