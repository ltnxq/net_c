#include<stdlib.h>
#include<stdio.h>

/*
1��checkpoint  ���ó�����ĳ����Ŀ�����Ϣ  ���Է��ص�ĳ��ʱ���ִ�� checkpoint ����һ���ӽ��������浱ʱ����Ŀ�����Ϣ
2��info checkpoint  �鿴��ǰcheckpoint����Ϣ
3��restart ��� ����ָ���ĳ�����յ�ִ��
*/

static int func()
{
	static int i = 0;
	++i;
	if (i == 2)
		return 1;
	return 0;
}

static int func3()
{
    return func();
}

static int func2()
{
    return func();
}

static int func1()
{
    return func();
}

int main()
{
    int ret = 0;

    ret += func1();
    ret += func2();
    ret += func3();

    return ret;
}
