#ifndef _CELL_STREAM_H_
#define _CELL_STREAM_H_

#include <cstdint>
#include <string>

#include "Cell.hpp"

class _X_UTILITY_API_ CELLStream
{
public:
	CELLStream(int size = 1024);
	CELLStream(char* pData, int size, bool bDelete = false);
	virtual ~CELLStream();

	char* data();
	int length();

	char ReadChar();
	bool ReadBool();
	int8_t ReadInt8();
	uint8_t ReadUint8();
	int16_t ReadInt16();
	uint16_t ReadUint16();
	int32_t ReadInt32();
	uint32_t ReadUint32();
	float ReadFloat();
	double ReadDouble();
	bool ReadString(std::string& str);

	uint32_t ReadArrayChar(char* data, uint32_t len);
	uint32_t ReadArrayBool(bool* data, uint32_t len);
	uint32_t ReadArrayInt8(int8_t* data, uint32_t len);
	uint32_t ReadArrayUint8(uint8_t* data, uint32_t len);
	uint32_t ReadArrayInt16(int16_t* data, uint32_t len);
	uint32_t ReadArrayUint16(uint16_t* data, uint32_t len);
	uint32_t ReadArrayInt32(int32_t* data, uint32_t len);
	uint32_t ReadArrayUint32(uint32_t* data, uint32_t len);
	uint32_t ReadArrayFloat(float* data, uint32_t len);
	uint32_t ReadArrayDouble(double* data, uint32_t len);


	bool WriteChar(const char data);
	bool WriteBool(const bool data);
	bool WriteInt8(const int8_t data);
	bool WriteUint8(const uint8_t data);
	bool WriteInt16(const int16_t data);
	bool WriteUint16(const uint16_t data);
	bool WriteInt32(const int32_t data);
	bool WriteUint32(const uint32_t data);
	bool WriteFloat(const float data);
	bool WriteDouble(const double data);
	bool WriteString(std::string& str);

	bool WriteArrayChar(const char* data, uint32_t len);
	bool WriteArrayBool(const bool* data, uint32_t len);
	bool WriteArrayInt8(const int8_t* data, uint32_t len);
	bool WriteArrayUint8(const uint8_t* data, uint32_t len);
	bool WriteArrayInt16(const int16_t* data, uint32_t len);
	bool WriteArrayUint16(const uint16_t* data, uint32_t len);
	bool WriteArrayInt32(const int32_t* data, uint32_t len);
	bool WriteArrayUint32(const uint32_t* data, uint32_t len);
	bool WriteArrayFloat(const float* data, uint32_t len);
	bool WriteArrayDouble(const double* data, uint32_t len);

	template<typename T>
	bool OnlyRead(T& data);

	inline bool CanRead(int n);
	inline bool CanWrite(int n);

protected:
	inline void _Push(int n);
	inline void _Pop(int n);
	inline void _SetWritePos(int n);

	template<typename T>
	bool _Read(T& data, bool offset = true);

	template<typename T>
	uint32_t _ReadArray(T* data, uint32_t len);

	template<typename T>
	bool _Write(const T data);

	template<typename T>
	bool _WriteArray(const T* data, uint32_t len);

private:
	char* _pBuf = nullptr;
	int _nSize = 0;
	int _nWritePos = 0;
	int _nReadPos = 0;
	bool _bDelete = true;
};


template<typename T>
inline bool CELLStream::OnlyRead(T& data)
{
	return _Read(data, false);
}

template<typename T>
inline bool CELLStream::_Read(T& data, bool offset)
{
	auto len = sizeof(T);

	if (CanRead(len) == false)
		return false;

	memcpy(&data, _pBuf + _nReadPos, len);

	if (offset)
		_nReadPos += len;

	return true;
}

template<typename T>
inline uint32_t CELLStream::_ReadArray(T* data, uint32_t len)
{
	uint32_t len1 = 0;
	auto ret = _Read(len1, false);
	if (ret == false)
		return 0;

	if (len < len1)
		return 0;

	auto nLen = len1 * sizeof(T);
	if (CanRead(nLen + sizeof(uint32_t)) == false)
		return 0;

	_nReadPos += sizeof(uint32_t);
	memcpy(data, _pBuf + _nReadPos, nLen);
	_nReadPos += nLen;
	return len1;
}

template<typename T>
inline bool CELLStream::_Write(const T data)
{
	auto nLen = sizeof(T);

	if (CanWrite(nLen) == false)
		return false;

	memcpy(_pBuf + _nWritePos, &data, nLen);
	_nWritePos += nLen;
	return true;
}

template<typename T>
inline bool CELLStream::_WriteArray(const T* data, uint32_t len)
{
	auto nLen = sizeof(T) * len;
	if (CanWrite(nLen + sizeof(uint32_t)) == false)
		return false;

	_Write(len);
	memcpy(_pBuf + _nWritePos, data, nLen);
	_nWritePos += nLen;
	return true;
}

#endif // !_CELL_STREAM_H_
