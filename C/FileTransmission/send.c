/*
Client, serve as a sender
*/

#include <stdio.h>
#include <stdlib.h>
#include <WINSOCK2.H>
#include <WS2tcpip.h>
#include "constant.h"
#pragma  comment(lib,"ws2_32")

SOCKET ConnectTo(char * szServerIP, int iPort);
int SendFile(FILE * pSendFile, SOCKET sClient);

SOCKET ConnectTo(char * szServerIP, int iPort)
{
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	SOCKET sClient;
	struct sockaddr_in serAddr;

	//绑定IP地址和端口号
	serAddr.sin_family = AF_INET;
	serAddr.sin_port = htons(iPort);
	inet_pton(serAddr.sin_family, szServerIP, &(serAddr.sin_addr.S_un.S_addr));

	if (WSAStartup(sockVersion, &data) != 0)
		return 0;

	if (INVALID_SOCKET == (sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)))
		return sClient;

	if (SOCKET_ERROR == connect(sClient, (struct sockaddr *)&serAddr, sizeof(serAddr)))
		closesocket(sClient);

	return sClient;
}

int SendFile(FILE * pSendFile, SOCKET sClient)
{
	static int iSendBufferSize;
	static int optlen = sizeof(int);
	size_t iSendSize = 0;
	char * pSendBuffer;

	getsockopt(sClient, SOL_SOCKET, SO_SNDBUF, (char *)&iSendBufferSize, &optlen);
	pSendBuffer = malloc(iSendBufferSize);

	while (!feof(pSendFile))
	{
		iSendSize = fread_s(pSendBuffer, iSendBufferSize, 1, iSendBufferSize, pSendFile);
		if (SOCKET_ERROR == send(sClient, pSendBuffer, (int)iSendSize, 0))
			return 1;
	}

	free(pSendBuffer);
	pSendBuffer = NULL;

	return 0;
}