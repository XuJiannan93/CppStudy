#ifndef _CELL_MESSAGE_STREAM_H_
#define _CELL_MESSAGE_STREAM_H_

#include "CELLStream.h"
#include "MessageHeader.hpp"

#include <string>

class CELLReadStream : public CELLStream
{
public:
	CELLReadStream(char* pData, int nSize, bool bDelete = false);
	CELLReadStream(netmsg_DataHeader* header);

	uint16_t GetNetCMD();
};

class CELLWriteStream : public CELLStream
{
public:
	CELLWriteStream(int size = 1024);
	CELLWriteStream(char* pData, int size, bool bDelete = false);

	void Finish();
	void SetNetCMD(uint16_t cmd);
	bool WriteString(const char* str, int len);
	bool WriteString(const char* str);
	bool WriteString(std::string& str);
};

#endif // !_CELL_MESSAGE_STREAM_H_
