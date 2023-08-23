#include<stdlib.h>
#include<stdio.h>

/*
1、checkpoint  设置程序中某个点的快照信息  可以返回到某个时间点执行 checkpoint 创建一个子进程来保存当时程序的快照信息
2、info checkpoint  查看当前checkpoint的信息
3、restart 编号 程序恢复到某个快照点执行
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
