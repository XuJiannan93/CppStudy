#ifndef _CELL_TIMESTAMP_H_
#define _CELL_TIMESTAMP_H_

#include <chrono>
using namespace std::chrono;

class __declspec(dllexport) CELLTimestamp
{
public:
	CELLTimestamp();
	virtual ~CELLTimestamp();

	void Update();
	double GetElapsedSecond();
	double GetElapsedTimeInMilliSec();
	long long GetElapsedTimeInMicroSec();

protected:
	time_point<high_resolution_clock> _begin;
};

#endif // !_CELL_TIMESTAMP_H_
