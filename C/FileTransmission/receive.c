/*
Server, serve as a receiver
*/

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#include "constant.h"

#pragma comment(lib,"ws2_32")

SOCKET ForConnect(int iPort, char * szClientIP, int iBufferSize);
int RecvFile(FILE * pRecvFile, SOCKET sClient);

SOCKET ForConnect(int iPort, char * szClientIP, int iBufferSize)
{
	//初始化WSA
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsaData;

	SOCKET sListen;
	SOCKET sClient;
	struct sockaddr_in sin;
	struct sockaddr_in remoteAddr;
	int nAddrlen = sizeof(remoteAddr);

	//绑定IP和端口
	sin.sin_family = AF_INET;
	sin.sin_port = htons(iPort);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;

	if (WSAStartup(sockVersion, &wsaData) != 0)
		return 0;

	//创建套接字
	sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sListen == INVALID_SOCKET)
	{
		fprintf(stderr, "Socket error !\n");
		return sListen;
	}

	if (bind(sListen, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		fprintf(stderr, "Bind error !");
		return sListen;
	}

	//开始监听
	if (listen(sListen, 5) == SOCKET_ERROR)
	{
		fprintf(stderr, "Listen error !");
		return sListen;
	}

	//接受连接
	while (1)
	{
		printf("Waiting for connection...\n");
		sClient = accept(sListen, (SOCKADDR *)&remoteAddr, &nAddrlen);
		if (sClient == INVALID_SOCKET)
		{
			fprintf(stderr, "Accept error !\n");
			continue;
		}
		inet_ntop(sin.sin_family, &(remoteAddr.sin_addr), szClientIP, iBufferSize);
		break;
	}
	closesocket(sListen);
	return sClient;
}

int RecvFile(FILE * pRecvFile, SOCKET sClient)
{
	static int iRecvBufferSize;
	static int optlen = sizeof(int);
	int iRecvSize;
	char * pRecvBuffer;

	getsockopt(sClient, SOL_SOCKET, SO_RCVBUF, (char *)&iRecvBufferSize, &optlen);
	pRecvBuffer = (char *)malloc(iRecvBufferSize);

	while ((iRecvSize = recv(sClient, pRecvBuffer, iRecvBufferSize, 0)) > 0)
		fwrite(pRecvBuffer, 1, iRecvSize, pRecvFile);

	free(pRecvBuffer);
	pRecvBuffer = NULL;
	return 0;
}