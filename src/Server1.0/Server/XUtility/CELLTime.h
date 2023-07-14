#ifndef _CELL_TIME_H_
#define _CELL_TIME_H_

#include <chrono>

class __declspec(dllexport) CELLTime
{
public:
	static time_t GetNowInMilliSec();
};

#endif // !_CELL_TIME_H_
