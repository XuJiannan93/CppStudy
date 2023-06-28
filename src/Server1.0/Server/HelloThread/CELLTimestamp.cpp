#include "CELLTimestamp.h"

CELLTimestamp::CELLTimestamp()
{
	Update();
}

CELLTimestamp::~CELLTimestamp()
{
}

void CELLTimestamp::Update()
{
	_begin = high_resolution_clock::now();
}

double CELLTimestamp::GetElapsedSecond()
{
	return this->GetElapsedTimeInMicroSec() * 0.000001;
}

double CELLTimestamp::GetElapsedTimeInMilliSec()
{
	return this->GetElapsedTimeInMicroSec() * 0.001;
}

long long CELLTimestamp::GetElapsedTimeInMicroSec()
{
	return duration_cast<microseconds>(high_resolution_clock::now() - _begin).count();
}