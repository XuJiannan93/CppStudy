#include "CELLStream.h"
#include "CELLStream.h"

#include <stdio.h>
#include <string.h>

CELLStream::CELLStream(int size)
{
	_nSize = size;
	_pBuf = new char[_nSize];
	_bDelete = true;
}

CELLStream::CELLStream(char* pData, int size, bool bDelete)
{
	_nSize = size;
	_pBuf = pData;
	_bDelete = bDelete;
}

CELLStream::~CELLStream()
{
	if (_pBuf == nullptr)
		return;

	if (_bDelete)
		delete[] _pBuf;
	_pBuf = nullptr;
}

char* CELLStream::data()
{
	return _pBuf;
}

int CELLStream::length()
{
	return _nWritePos;
}

char CELLStream::ReadChar()
{
	char def = 0;
	auto ret = _Read(def);
	if (ret == false)
		printf("[WARRING] can not read char in stream!");
	return def;
}

bool CELLStream::ReadBool()
{
	bool def = false;
	auto ret = _Read(def);
	if (ret == false)
		printf("[WARRING] can not read bool in stream!");
	return def;
}

int8_t CELLStream::ReadInt8()
{
	int8_t def = 0;
	auto ret = _Read(def);
	if (ret == false)
		printf("[WARRING] can not read int8 in stream!");
	return def;
}

uint8_t CELLStream::ReadUint8()
{
	uint8_t def = 0;
	auto ret = _Read(def);
	if (ret == false)
		printf("[WARRING] can not read uint8 in stream!");
	return def;
}

int16_t CELLStream::ReadInt16()
{
	int16_t def = 0;
	auto ret = _Read(def);
	if (ret == false)
		printf("[WARRING] can not read int16 in stream!");
	return def;
}

uint16_t CELLStream::ReadUint16()
{
	uint16_t def = 0;
	auto ret = _Read(def);
	if (ret == false)
		printf("[WARRING] can not read uint16 in stream!");
	return def;
}

int32_t CELLStream::ReadInt32()
{
	int32_t def = 0;
	auto ret = _Read(def);
	if (ret == false)
		printf("[WARRING] can not read int32 in stream!");
	return def;
}

uint32_t CELLStream::ReadUint32()
{
	uint32_t def = 0;
	auto ret = _Read(def);
	if (ret == false)
		printf("[WARRING] can not read uint32 in stream!");
	return def;
}

float CELLStream::ReadFloat()
{
	float def = 0.0f;
	auto ret = _Read(def);
	if (ret == false)
		printf("[WARRING] can not read float in stream!");
	return def;
}

double CELLStream::ReadDouble()
{
	double def = 0.0;
	auto ret = _Read(def);
	if (ret == false)
		printf("[WARRING] can not read double in stream!");
	return def;
}

bool CELLStream::ReadString(std::string& str)
{
	uint32_t len = 0;
	_Read(len, false);
	if (len > 0)
	{
		if (CanRead(len + sizeof(uint32_t)))
		{
			_Pop(sizeof(uint32_t));
			str.insert(0, _pBuf + _nReadPos, len);
			_Pop(len);
			return true;
		}
	}
	return false;
}

uint32_t CELLStream::ReadArrayChar(char* data, uint32_t len)
{
	return	_ReadArray<char>(data, len);
}

uint32_t CELLStream::ReadArrayBool(bool* data, uint32_t len)
{
	return	_ReadArray<bool>(data, len);
}

uint32_t CELLStream::ReadArrayInt8(int8_t* data, uint32_t len)
{
	return	_ReadArray<int8_t>(data, len);
}

uint32_t CELLStream::ReadArrayUint8(uint8_t* data, uint32_t len)
{
	return	_ReadArray<uint8_t>(data, len);
}

uint32_t CELLStream::ReadArrayInt16(int16_t* data, uint32_t len)
{
	return	_ReadArray<int16_t>(data, len); return 0;
}

uint32_t CELLStream::ReadArrayUint16(uint16_t* data, uint32_t len)
{
	return	_ReadArray<uint16_t>(data, len);
}

uint32_t CELLStream::ReadArrayInt32(int32_t* data, uint32_t len)
{
	return	_ReadArray<int32_t>(data, len);
}

uint32_t CELLStream::ReadArrayUint32(uint32_t* data, uint32_t len)
{
	return	_ReadArray<uint32_t>(data, len);
}

uint32_t CELLStream::ReadArrayFloat(float* data, uint32_t len)
{
	return	_ReadArray<float>(data, len);
}

uint32_t CELLStream::ReadArrayDouble(double* data, uint32_t len)
{
	return	_ReadArray<double>(data, len);
}

bool CELLStream::WriteChar(const char data)
{
	return _Write(data);
}

bool CELLStream::WriteBool(const bool data)
{
	return _Write(data);
}

bool CELLStream::WriteInt8(int8_t data)
{
	return _Write(data);
}

bool CELLStream::WriteUint8(const uint8_t data)
{
	return _Write(data);
}

bool CELLStream::WriteInt16(const int16_t data)
{
	return _Write(data);
}

bool CELLStream::WriteUint16(const uint16_t data)
{
	return _Write(data);
}

bool CELLStream::WriteInt32(const int32_t data)
{
	return _Write(data);
}

bool CELLStream::WriteUint32(const uint32_t data)
{
	return _Write(data);
}

bool CELLStream::WriteFloat(const float data)
{
	return _Write(data);
}

bool CELLStream::WriteDouble(const double data)
{
	return _Write(data);
}

bool CELLStream::WriteString(std::string& str)
{
	auto data = str.data();
	return WriteArrayChar(data, str.length());
}

bool CELLStream::WriteArrayChar(const char* data, uint32_t len)
{
	return _WriteArray<char>(data, len);
}

bool CELLStream::WriteArrayBool(const bool* data, uint32_t len)
{
	return _WriteArray<bool>(data, len);
}

bool CELLStream::WriteArrayInt8(const int8_t* data, uint32_t len)
{
	return _WriteArray<int8_t>(data, len);
}

bool CELLStream::WriteArrayUint8(const uint8_t* data, uint32_t len)
{
	return _WriteArray<uint8_t>(data, len);
}

bool CELLStream::WriteArrayInt16(const int16_t* data, uint32_t len)
{
	return _WriteArray<int16_t>(data, len);
}

bool CELLStream::WriteArrayUint16(const uint16_t* data, uint32_t len)
{
	return _WriteArray<uint16_t>(data, len);
}

bool CELLStream::WriteArrayInt32(const int32_t* data, uint32_t len)
{
	return _WriteArray<int32_t>(data, len);
}

bool CELLStream::WriteArrayUint32(const uint32_t* data, uint32_t len)
{
	return _WriteArray<uint32_t>(data, len);
}

bool CELLStream::WriteArrayFloat(const float* data, uint32_t len)
{
	return _WriteArray<float>(data, len);
}

bool CELLStream::WriteArrayDouble(const double* data, uint32_t len)
{
	return _WriteArray<double>(data, len);
}

inline bool CELLStream::CanRead(int n)
{
	return n <= _nSize - _nReadPos;
}

inline bool CELLStream::CanWrite(int n)
{
	return n <= _nSize - _nWritePos;
}

inline void CELLStream::_Push(int n)
{
	_nWritePos += n;
}

inline void CELLStream::_Pop(int n)
{
	_nReadPos += n;
}

inline void CELLStream::_SetWritePos(int n)
{
	_nWritePos = n;
}
