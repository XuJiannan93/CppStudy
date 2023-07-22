#include "UnityTcpClient.h"
#include "CELLMsgStream.h"

extern "C"
{
	EXPORT_DLL void* CELLClient_Create(void* obj, OnNetMsgCallback cb, int sendSize, int recvSize)
	{
		if (obj == nullptr) return nullptr;
		UnityTcpClient* pClient = new UnityTcpClient();
		pClient->SetCallback(obj, cb);
		pClient->InitSocket(sendSize, recvSize);
		return pClient;
	}

	EXPORT_DLL bool CELLClient_Connect(UnityTcpClient* pClient, const char* ip, short port)
	{
		if (pClient == nullptr) return false;
		if (ip == nullptr) return false;
		return pClient->Connect(ip, port) != SOCKET_ERROR;
	}

	EXPORT_DLL bool CELLClient_OnRun(UnityTcpClient* pClient)
	{
		if (pClient == nullptr) return false;
		return pClient->OnRun();
	}

	EXPORT_DLL void CELLClient_Close(UnityTcpClient* pClient)
	{
		if (pClient == nullptr) return;
		pClient->Close();
		delete pClient;
	}

	EXPORT_DLL int CELLClient_SendData(UnityTcpClient* pClient, const char* data, int len)
	{
		if (pClient == nullptr) return 0;
		return pClient->SendData(data, len);
	}

	EXPORT_DLL int CELLClient_SendStream(UnityTcpClient* pClient, CELLWriteStream* stream)
	{
		if (pClient == nullptr)
			return 0;
		if (stream == nullptr)
			return 0;

		stream->Finish();
		return pClient->SendData(stream->data(), stream->length());
	}

	EXPORT_DLL void* CELLWriteStream_Create(int nSize)
	{
		CELLWriteStream* pStream = new CELLWriteStream(nSize);
		return pStream;
	}

	EXPORT_DLL bool CELLWriteStream_WriteInt8(CELLWriteStream* stream, int8_t n)
	{
		if (stream == nullptr)
			return false;

		return stream->WriteInt8(n);
	}

	EXPORT_DLL bool CELLWriteStream_WriteInt16(CELLWriteStream* stream, int16_t n)
	{
		if (stream == nullptr)
			return false;

		return stream->WriteInt16(n);
	}

	EXPORT_DLL bool CELLWriteStream_WriteInt32(CELLWriteStream* stream, int32_t n)
	{
		if (stream == nullptr)
			return false;

		return stream->WriteInt32(n);
	}

	EXPORT_DLL bool CELLWriteStream_WriteUInt8(CELLWriteStream* stream, uint8_t n)
	{
		if (stream == nullptr)
			return false;

		return stream->WriteUint8(n);
	}

	EXPORT_DLL bool CELLWriteStream_WriteUInt16(CELLWriteStream* stream, uint16_t n)
	{
		if (stream == nullptr)
			return false;

		return stream->WriteUint16(n);
	}

	EXPORT_DLL bool CELLWriteStream_WriteUInt32(CELLWriteStream* stream, uint32_t n)
	{
		if (stream == nullptr)
			return false;

		return stream->WriteUint32(n);
	}

	EXPORT_DLL bool CELLWriteStream_WriteFloat(CELLWriteStream* stream, float n)
	{
		if (stream == nullptr)
			return false;

		return stream->WriteFloat(n);
	}

	EXPORT_DLL bool CELLWriteStream_WriteDouble(CELLWriteStream* stream, double n)
	{
		if (stream == nullptr)
			return false;

		return stream->WriteDouble(n);
	}

	EXPORT_DLL bool CELLWriteStream_WriteString(CELLWriteStream* stream, char* n)
	{
		if (stream == nullptr)
			return false;

		return stream->WriteString(n);
	}

	EXPORT_DLL void CELLWriteStream_Release(CELLWriteStream* stream)
	{
		if (stream == nullptr)
			return;

		delete stream;
	}

	EXPORT_DLL void* CELLReadStream_Create(char* data, int len)
	{
		CELLReadStream* stream = new CELLReadStream(data, len);
		return stream;
	}

	EXPORT_DLL int8_t CELLReadStream_ReadInt8(CELLReadStream* stream)
	{
		if (stream == nullptr)
			return 0;

		return stream->ReadInt8();
	}

	EXPORT_DLL int16_t CELLReadStream_ReadInt16(CELLReadStream* stream)
	{
		if (stream == nullptr)
			return 0;

		return stream->ReadInt16();
	}

	EXPORT_DLL int32_t CELLReadStream_ReadInt32(CELLReadStream* stream)
	{
		if (stream == nullptr)
			return 0;

		return stream->ReadInt32();
	}

	EXPORT_DLL uint8_t CELLReadStream_ReadUInt8(CELLReadStream* stream)
	{
		if (stream == nullptr)
			return 0;

		return stream->ReadUint8();
	}

	EXPORT_DLL uint16_t CELLReadStream_ReadUInt16(CELLReadStream* stream)
	{
		if (stream == nullptr)
			return 0;

		return stream->ReadUint16();
	}

	EXPORT_DLL uint32_t CELLReadStream_ReadUInt32(CELLReadStream* stream)
	{
		if (stream == nullptr)
			return 0;

		return stream->ReadUint32();
	}

	EXPORT_DLL float CELLReadStream_ReadFloat(CELLReadStream* stream)
	{
		if (stream == nullptr)
			return 0.0f;

		return stream->ReadFloat();
	}

	EXPORT_DLL double CELLReadStream_ReadDouble(CELLReadStream* stream)
	{
		if (stream == nullptr)
			return 0.0;

		return stream->ReadDouble();
	}

	EXPORT_DLL uint32_t CELLReadStream_OnlyReadUInt32(CELLReadStream* stream)
	{
		if (stream == nullptr)
			return 0;

		uint32_t len = 0;
		stream->OnlyRead(len);
		return len;
	}

	EXPORT_DLL bool CELLReadStream_ReadString(CELLReadStream* stream, char* buffer, int len)
	{
		if (stream == nullptr)
			return false;

		if (buffer == nullptr)
			return false;

		return stream->ReadArrayChar(buffer, len);
	}

	EXPORT_DLL void CELLReadStream_Release(CELLReadStream* stream)
	{
		if (stream == nullptr)
			return;

		delete stream;
	}
}

