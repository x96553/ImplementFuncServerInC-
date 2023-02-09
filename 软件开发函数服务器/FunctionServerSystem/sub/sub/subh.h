#ifdef dllAPI
#else
#define dllAPI extern "C" _declspec(dllimport)
#endif //dllAPI
dllAPI double sub(double a, double b);