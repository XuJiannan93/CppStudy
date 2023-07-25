#ifndef _CELL_FD_SET_H_
#define _CELL_FD_SET_H_

#define CELL_MAX_FD 10240

#include "Cell.hpp"

class _X_UTILITY_API_ CELLFDSet
{
public:
	CELLFDSet();
	~CELLFDSet();

	inline void Set(SOCKET sock);
	inline void Clear(SOCKET sock);
	inline void Zero();
	inline bool IsSet(SOCKET sock);
	inline fd_set* Get();
	inline void Copy(CELLFDSet& set);

private:
	fd_set* _pfdset = nullptr;
	size_t _nfdSize = 0;
};

#endif // !_CELL_FD_SET_H_
