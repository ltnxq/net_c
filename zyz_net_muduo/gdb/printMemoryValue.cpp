#include<stdio.h>

/*
x/nfu addr �����ӡ�ڴ�ֵ
   n-�����Ԫ�ĸ���
   f-�����ʽ x 16  o 8  u 10  t 2
   u-һ����Ԫ�ĳ���  b һ��byte h����byte w���ĸ�byte

*/

//x/nfu addr �����ӡ�ڴ�ֵ
  


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
