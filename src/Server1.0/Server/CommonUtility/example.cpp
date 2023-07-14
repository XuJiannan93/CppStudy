#ifndef _CU_EXAMPLE_CPP_H_
#define _CU_EXAMPLE_CPP_H_

#include<string>

extern "C"
{
	int _declspec(dllexport) Add(int a, int b)
	{
		return a + b;
	}

	typedef void(*Callback)(const char* str);

	void _declspec(dllexport) TestCallback(const char* str, Callback cb)
	{
		std::string s = "HELLO ";
		s += str;
		cb(s.c_str());
	}
}

#endif // !_CU_EXAMPLE_CPP_H_
