#ifndef _CELL_TASK_H_
#define _CELL_TASK_H_

#include "Cell.hpp"

class _X_UTILITY_API_ CELLTask
{
public:
	CELLTask();
	virtual ~CELLTask();

private:

public:
	virtual void DoTask();
};
#endif // !_CELL_TASK_H_