#include<stdlib.h>
#include<stdio.h>
#include"SDK.h"
int main()
{
	double a = 22.322, b = 95.5134;
	char add[] = "add";
	printf("%f\n", *(double*)SDK::FuncSCallProxy1(add, 2, 'f', sizeof(double), &a, 'f', sizeof(double), &b));
	system("pause");
	return 0;
}