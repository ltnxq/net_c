#pragma once

#include<windows.h>
#include<stdio.h>
#include<process.h>

unsigned WINAPI Read(void* arg);
unsigned WINAPI Accu(void* arg);

static HANDLE semOne;
static HANDLE semTwo;
static int num;

int sync_sema()
{
	HANDLE hThread1, hThread2;
	semOne = CreateSemaphore(NULL, 0, 1, NULL);
	semTwo = CreateSemaphore(NULL, 1, 1, NULL);

	//0-non-signaled状态
	hThread1 = (HANDLE)_beginthreadex(NULL, 0, Read, NULL, 0, NULL);
	hThread2 = (HANDLE)_beginthreadex(NULL, 0, Accu, NULL, 0, NULL);

	WaitForSingleObject(hThread1, INFINITE);
	WaitForSingleObject(hThread2, INFINITE);

	CloseHandle(semOne);
	CloseHandle(semTwo);

	return 0;


}
//信号量=0的时候，进入non-signaled状态，不等于0的时候进入signaled状态
//WaitForSingleObject 大于0的时候返回,返回的时候-1
//交替执行-->输入=》计算=》输入=》计算
unsigned WINAPI Read(void* arg)
{
	int i;
	for (i = 0; i < 5; i++) {
		fputs("Input num: ", stdout);
		WaitForSingleObject(semTwo, INFINITE);
		scanf("%d", &num);
		ReleaseSemaphore(semOne, 1, NULL);
	}
	return 0;

}

unsigned WINAPI Accu(void* arg)
{
	int sum = 0, i;
	for (i = 0; i < 5; i++) {
		WaitForSingleObject(semOne, INFINITE);
		sum += num;
		ReleaseSemaphore(semTwo, 1, NULL);
	}
	printf("Result:%d\n", sum);
	return 0;
}