/*
1、tbreak(tb) - 设置临时断点
2、break(b) 行号 if 条件
3、ignore 断点编号i cnt 来忽略断点
*/

#include<stdio.h>

int main()
{
	int i = 0;
	int sum = 0;

	for (i = 1; i <= 280; ++i)
	{
		sum += i;
	}

	printf("%d\n", sum);
	return 0;
}