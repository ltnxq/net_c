/*
1��tbreak(tb) - ������ʱ�ϵ�
2��break(b) �к� if ����
3��ignore �ϵ���i cnt �����Զϵ�
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