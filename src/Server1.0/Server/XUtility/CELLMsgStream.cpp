#include "CELLMsgStream.h"

CELLReadStream::CELLReadStream(
	char* pData, int nSize, bool bDelete)
	:CELLStream(pData, nSize, bDelete)
{
	_Push(nSize);
	ReadUint16();
	GetNetCMD();
}

CELLReadStream::CELLReadStream(netmsg_DataHeader* header)
	:CELLStream((char*)header, header->len)
{
	_Push(header->len);
	ReadUint16();
	GetNetCMD();
}


uint16_t CELLReadStream::GetNetCMD()
{
	uint16_t cmd = CMD_ERROR;
	cmd = ReadUint16();
	return cmd;
}

CELLWriteStream::CELLWriteStream(int size)
	:CELLStream(size)
{
	WriteUint16(0);
}

CELLWriteStream::CELLWriteStream(char* pData, int size, bool bDelete)
	:CELLStream(pData, size, bDelete)
{
	WriteUint16(0);
}

void CELLWriteStream::Finish()
{
	int pos = length();
	_SetWritePos(0);
	WriteUint16(pos);
	_SetWritePos(pos);
}

void CELLWriteStream::SetNetCMD(uint16_t cmd)
{
	WriteUint16(cmd);
}

bool CELLWriteStream::WriteString(const char* str, int len)
{
	return _WriteArray(str, len);
}

bool CELLWriteStream::WriteString(const char* str)
{
	return _WriteArray(str, strlen(str));
}

bool CELLWriteStream::WriteString(std::string& str)
{
	return _WriteArray(str.c_str(), str.length());
}
