#define dllAPI extern "C" _declspec(dllexport)
#include"addh.h"
dllAPI double add(double a, double b)
{
	return a + b;
}