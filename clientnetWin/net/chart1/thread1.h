#pragma once
#include<windows.h>
#include<stdio.h>
#include<process.h>

unsigned WINAPI ThreadFunc(void* arg);

int thread_1()
{
	HANDLE hThread;
	unsigned int threadId;
	int param = 5;
	DWORD dw;

	puts("_beginthreadex call");
	hThread = (HANDLE)_beginthreadex(NULL, 0, ThreadFunc, (void*)&param, 0, &threadId);
	if (hThread == NULL) {
		puts("_beginthreaddex error()");
		return -1;
	}

	//等待线程终止，linux 是 thread.join()函数
	if ((dw = WaitForSingleObject(hThread, INFINITE)) == WAIT_FAILED) {
		puts("THREAD WAIT ERROR");
		return -1;
	}

	printf("wait result:%s\n", (dw == WAIT_OBJECT_0) ? "signaled" : "time-out");
	puts("end of main");
	return 0;


}

unsigned WINAPI ThreadFunc(void* arg)
{
	int i;
	int cnt = (*(int*)arg);
	for (size_t i = 0; i < cnt; i++)
	{
		Sleep(1000);
		puts("running thread");
	}

	return 0;
}
