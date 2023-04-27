#pragma once

#include<windows.h>
#include<stdio.h>
#include<process.h>

#define NUM_THREAD 50
unsigned WINAPI threadInc(void* arg);
unsigned WINAPI threadDes(void* arg);

long long num = 0;
CRITICAL_SECTION cs;

int sync_cs()
{
	HANDLE handles[NUM_THREAD];
	int i;
	printf("sizeof long long:%d\n", num);
	//≥ı ºªØcs
	InitializeCriticalSection(&cs);
	for (size_t i = 0; i < NUM_THREAD; i++)
	{
		if (i % 2)
			handles[i] = (HANDLE)_beginthreadex(NULL, 0, threadInc, NULL, 0, NULL);
		else
			handles[i] = (HANDLE)_beginthreadex(NULL, 0, threadDes, NULL, 0, NULL);
	}

	WaitForMultipleObjects(NUM_THREAD, handles, true, INFINITE);
	//…æ≥˝CS
	DeleteCriticalSection(&cs);
	printf("result:%lld \n", num);
	return 0;
}

unsigned WINAPI threadInc(void* arg)
{
	int i;
	EnterCriticalSection(&cs);
	for (i = 0; i < 50000000; i++)
		num += 1;
	LeaveCriticalSection(&cs);
	return 0;
}

unsigned WINAPI threadDes(void* arg)
{
	int i;
	EnterCriticalSection(&cs);
	for (i = 0; i < 50000000; i++)
		num -= 1;
	LeaveCriticalSection(&cs);
	return 0;
}