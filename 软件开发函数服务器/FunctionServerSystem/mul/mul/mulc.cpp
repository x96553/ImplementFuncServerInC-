#define dllAPI extern "C" _declspec(dllexport)
#include"mulh.h"
dllAPI double mul(double a, double b)
{
	return a * b;
}