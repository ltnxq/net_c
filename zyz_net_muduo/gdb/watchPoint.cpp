#include<stdio.h>
#include<pthread.h>
#include<unistd.h>

/*
1��rwatch  ���� ������������ȡ��ʱ����ͣ����
2��awatch  ���� ����ȡ������ʱ�������ͣ
3��watch expr thread threadnum
4��info threads �鿴�̵߳���Ϣ
*/

int  a = 0;

void* thread1_func(void* p_arg)
{
	while (1)
	{
		printf("%d\n", a);
		sleep(10);
	}
}


void* thread2_func(void* p_arg)
{
	while (1)
	{
		a++;
		sleep(10);
	}
}

int main()
{
	pthread_t t1 ,t2;
	char threadName[10] = "thread 1";
	char threadName2[10] = "thread 2";

	pthread_create(&t1, NULL, thread1_func, threadName);
	pthread_create(&t2, NULL, thread2_func, threadName2);


	sleep(1000);

	return 0;
}