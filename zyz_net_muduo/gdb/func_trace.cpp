#include<stdio.h>
/*
1、list 行数 从开始查看源码  list 函数名 查看某个函数的源码
2、start 开始gdb调试
3、step (s) 进入某个函数
4、backtrace(bt)  查看函数调用的栈帧
   前面的#后面跟的是栈帧的编号
5、info locals 查看局部变量的信息
6、finish 让程序一直运行到从当前函数返回
7、在gdb中修改错误
   例如为add_range的sum 赋予初始值 add var sum = 0
8、在其他函数中查看别的函数的局部变量的值
   fram 1
   info locals
9、参考 https://zhuanlan.zhihu.com/p/100385553

*/


int add_range(int low, int high)
{
	int i, sum;
	for (i = low; i < high; ++i)
		sum += i;
	return sum;
}

int main()
{
	int result[100];
	result[0] = add_range(1, 10);
	result[1] = add_range(1, 100);
	printf("r[0]=%d\nr[1]=%d\n", result[0], result[1]);

	return 0;
}