#ifndef _X_PRINTF_ 
#define _X_PRINTF_
#ifdef _DEBUG
#include<stdio.h>
#define xPrintf(...) printf(__VA_ARGS__)
#else
#define xPrintf(...)
#endif // _DEBUG
#endif // !_X_PRINTF_ 
