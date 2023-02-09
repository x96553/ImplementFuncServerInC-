#define dllAPI extern "C" _declspec(dllexport)
#include"divh.h"
dllAPI double div(double a, double b)
{
	if (b == 0)
		return -1;
	return a / b;
}