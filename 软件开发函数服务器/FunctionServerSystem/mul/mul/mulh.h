#ifdef dllAPI
#else
#define dllAPI extern "C" _declspec(dllimport)
#endif //dllAPI
dllAPI double mul(double a, double b);