#include "CELLTime.h"

using namespace std::chrono;

time_t CELLTime::GetNowInMilliSec()
{
	return duration_cast<milliseconds>
		(high_resolution_clock::now().time_since_epoch()).count();
}
