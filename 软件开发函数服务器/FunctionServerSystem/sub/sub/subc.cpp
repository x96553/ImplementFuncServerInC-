#define dllAPI extern "C" _declspec(dllexport)
#include"subh.h"
dllAPI double sub(double a, double b)
{
	return a - b;
}