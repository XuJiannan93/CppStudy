#ifndef _CELL_HPP_
#define _CELL_HPP_

#ifdef _WIN32

#define FD_SETSIZE	65535 
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#include <Windows.h>
#include <WinSock2.h>
#pragma comment(lib,"ws2_32.lib")

#ifndef socklen_t
#define socklen_t int
#endif // !socklen_t

#else
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

#define SOCKET int
#define INVALID_SOCKET (SOCKET)(~0)
#define SOCKET_ERROR (-1)
#endif

#define _X_UTILITY_API_ __declspec(dllexport)

#ifndef  _RECV_BUFF_SIZE_
#define RECV_BUFF_SIZE 10240
#define SEND_BUFF_SIZE RECV_BUFF_SIZE
#endif // ! _RECV_BUFF_SIZE_

#endif // !_CELL_HPP_