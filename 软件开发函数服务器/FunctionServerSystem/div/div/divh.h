#ifdef dllAPI
#else
#define dllAPI extern "C" _declspec(dllimport)
#endif //dllAPI
dllAPI double div(double a, double b);
