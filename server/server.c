#ifndef __STDIO_H__
#define __STDIO_H__
#include <stdio.h>
#endif

#ifndef __STDLIB_H__
#define __STDLIB_H__
#include <stdlib.h>
#endif

#ifndef __STRING_H__
#define __STRING_H__
#include <string.h>
#endif

#ifndef __WINSOCK2_H__
#define __WINSOCK2_H__
#include <WinSock2.h>
#endif

extern WSADATA wsaData;
extern SOCKET hServSock;
extern SOCKET hClntSock;
extern SOCKADDR_IN servAddr;
extern SOCKADDR_IN clntAddr;
extern int szClntAddr;
extern char message[20];

int strLen;

void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void opensocket()
{
	int port = 0;

	printf("Port: ");
	scanf("%d", &port);

	// Load Winsock 2.2 DLL
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hServSock = socket(PF_INET, SOCK_STREAM, 0); // ���� ���� ����
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket() error!");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(port);

	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) // ���Ͽ� �ּ� �Ҵ�
		ErrorHandling("bind() error!");

	if (listen(hServSock, 5) == SOCKET_ERROR) // ���� ��û ��� ����
		ErrorHandling("listen() error!");

	szClntAddr = sizeof(clntAddr);
	hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr); // ���� ��û ����
	if (hClntSock == INVALID_SOCKET)
		ErrorHandling("accept() error!");
}

void freesocket()
{
	closesocket(hClntSock); // ���� ����
	WSACleanup();
}

void sendmessage(char *message)
{
	send(hClntSock, message, 19/*sizeof(message)*/, 0); // ������ ����
}

void receivemessage()
{
	// ������ ����
	strLen = recv(hClntSock, message, sizeof(message) - 1, 0);
	if (strLen == -1)
		ErrorHandling("read() error!");
	message[strLen] = 0;
	printf("Message from server: %s\n", message);
}