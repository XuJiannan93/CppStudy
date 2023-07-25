#include "CELLFDSet.h"

CELLFDSet::CELLFDSet()
{
	int nSocketNum = CELL_MAX_FD;
#ifdef _WIN32
	_nfdSize = sizeof(u_int) + sizeof(SOCKET) * nSocketNum;
#else
	nfdSize = nSocketNum / (8 * sizeof(char));
#endif // _WIN32
	_pfdset = (fd_set*)new char[_nfdSize];
	memset(_pfdset, 0, _nfdSize);

}

CELLFDSet::~CELLFDSet()
{
	if (_pfdset == nullptr)
		return;

	delete _pfdset;
	_pfdset = nullptr;
}

inline void CELLFDSet::Set(SOCKET sock)
{
#ifdef _WIN32
	FD_SET(sock, _pfdset);
#else
	if (sock < CELL_MAX_FD)
		FD_SET(sock, _pfdset);
	else
		CELLLog_Error("CELLFDSet::Set() fd value[%d] can not bigger than %d", (int)sock, CELL_MAX_FD);

#endif // _WIN32
}

inline void CELLFDSet::Clear(SOCKET sock)
{
	FD_CLR(sock, _pfdset);
}

inline void CELLFDSet::Zero()
{
#ifdef  _WIN32
	FD_ZERO(_pfdset);
#else
	memset(_pfdset, 0, _nfdSize);
#endif //  _WIN32
}

inline bool CELLFDSet::IsSet(SOCKET sock)
{
	return FD_ISSET(sock, _pfdset);
}

inline fd_set* CELLFDSet::Get()
{
	return _pfdset;
}

inline void CELLFDSet::Copy(CELLFDSet& set)
{
	memcpy(_pfdset, set.Get(), set._nfdSize);
}
