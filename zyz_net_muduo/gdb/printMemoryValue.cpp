#include<stdio.h>

/*
x/nfu addr 命令打印内存值
   n-输出单元的个数
   f-输出格式 x 16  o 8  u 10  t 2
   u-一个单元的长度  b 一个byte h两个byte w是四个byte

*/

//x/nfu addr 命令打印内存值
  


int main(int argc, char* argv[])
{
	int i = 0;
	int a[100];

	for (i = 0; i < static_cast<int>(sizeof(a)); ++i)
	{
		a[i] = i;
	}

	return 0;
}
