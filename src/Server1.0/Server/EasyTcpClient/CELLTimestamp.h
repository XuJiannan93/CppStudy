#ifndef CELL_TIMESTAMP_H
#define CELL_TIMESTAMP_H

#include <chrono>
using namespace std::chrono;

class CELLTimestamp
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

#endif // !CELL_TIMESTAMP_H
