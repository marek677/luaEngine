#if defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
	#include <windows.h> //Sleep function...
#else
	#define Sleep sleep
#endif