#ifndef _CU_UTILITY_CPP_
#define _CU_UTILITY_CPP_

#include <chrono>
#include <thread>

namespace util
{
	void _declspec(dllexport) Sleep(time_t dt)
	{
		std::chrono::milliseconds t(dt);
		std::this_thread::sleep_for(t);
	}
}


#endif // !_CU_UTILITY_CPP_
