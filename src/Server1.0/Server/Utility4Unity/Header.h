#ifndef _CPP_NET_DLL_H_
#define _CPP_NET_DLL_H_

#ifdef _WIN32
#define EXPORT_DLL _declspec(dllexport)
#else
#define EXPORT_DLL
#endif // _WIN32

extern "C"
{
	typedef void(*OnNetMsgCallback)(void* csObj, void* data, int len);
}

#endif // !_CPP_NET_DLL_H_

