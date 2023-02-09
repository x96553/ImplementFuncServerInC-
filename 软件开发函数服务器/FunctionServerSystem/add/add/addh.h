#ifdef dllAPI
#else
#define dllAPI extern "C" _declspec(dllimport)
#endif //dllAPI
dllAPI double add(double a, double b);