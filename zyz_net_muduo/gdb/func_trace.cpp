#include<stdio.h>
/*
1��list ���� �ӿ�ʼ�鿴Դ��  list ������ �鿴ĳ��������Դ��
2��start ��ʼgdb����
3��step (s) ����ĳ������
4��backtrace(bt)  �鿴�������õ�ջ֡
   ǰ���#���������ջ֡�ı��
5��info locals �鿴�ֲ���������Ϣ
6��finish �ó���һֱ���е��ӵ�ǰ��������
7����gdb���޸Ĵ���
   ����Ϊadd_range��sum �����ʼֵ add var sum = 0
8�������������в鿴��ĺ����ľֲ�������ֵ
   fram 1
   info locals
9���ο� https://zhuanlan.zhihu.com/p/100385553

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