#pragma once

#include<windows.h>
#include<stdio.h>
#include<process.h>

#define NUM_THREAD 50
unsigned WINAPI threadInc(void* arg);
unsigned WINAPI threadDes(void* arg);

long long num = 0;
HANDLE mutex;

int sync_mutex()
{
	HANDLE handles[NUM_THREAD];
	int i;
	printf("sizeof long long:%d\n", num);
	
	//创建mutex
	mutex = CreateMutex(NULL, false, NULL);
	for (size_t i = 0; i < NUM_THREAD; i++)
	{
		if (i % 2)
			handles[i] = (HANDLE)_beginthreadex(NULL, 0, threadInc, NULL, 0, NULL);
		else
			handles[i] = (HANDLE)_beginthreadex(NULL, 0, threadDes, NULL, 0, NULL);
	}

	WaitForMultipleObjects(NUM_THREAD, handles, true, INFINITE);
	
	//销毁mutex,调用
	CloseHandle(mutex);
	printf("result:%lld \n", num);
	return 0;
}

unsigned WINAPI threadInc(void* arg)
{
	int i;
	WaitForSingleObject(mutex, INFINITE);
	for (i = 0; i < 50000000; i++)
		num += 1;
	ReleaseMutex(mutex);
	return 0;
}

unsigned WINAPI threadDes(void* arg)
{
	int i;
	WaitForSingleObject(mutex, INFINITE);
	for (i = 0; i < 50000000; i++)
		num -= 1;
	ReleaseMutex(mutex);
	return 0;
}