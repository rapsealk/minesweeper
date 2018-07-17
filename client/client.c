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
extern SOCKET hSocket;
extern char message[20];
extern int strLen;
extern SOCKADDR_IN servAddr;

void ErrorHandling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

void opensocket()
{
	char address[20] = { '\0', };
	int port = 0;

	printf("IP Address: ");
	scanf("%s", address);
	printf("Port: ");
	scanf("%d", &port);

	// Load WinSocket 2.2 DLL
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	// 서버 접속을 위한 소켓 생성
	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("hSocket() error!");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(address);
	servAddr.sin_port = htons(port);
	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() error!");
}

void freesocket()
{
	closesocket(hSocket); // 연결 종료
	WSACleanup();
}

void sendmessage()
{
	send(hSocket, message, sizeof(message), 0); // 데이터 전송
}

void receivemessage()
{
	// 데이터 수신
	strLen = recv(hSocket, message, 20/*sizeof(message)*/ - 1, 0);
	if (strLen == -1)
		ErrorHandling("read() error!");
	message[strLen] = 0;
	//printf("Message from server: %s\n", message);
}