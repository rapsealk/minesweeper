#ifndef __STDIO_H__
#define __STDIO_H__
#include <stdio.h>
#endif

#ifndef __STDLIB_H__
#define __STDLIB_H__
#include <stdlib.h>
#endif

#ifndef __TIME_H__
#define __TIME_H__
#include <time.h>
#endif

#ifndef __WINSOCK2_H__
#define __WINSOCK2_H__
#include <WinSock2.h>
#endif

#ifndef __MINE_H__
#define __MINE_H__
#include "MINE.h"
#endif

/*
#ifndef __SCORE_H__
#define __SCORE_H__
#include "SCORE.h"
#endif
*/

#ifndef __CLIENT_H__
#define __CLIENT_H__
#include "client.h"
#endif

// Client
WSADATA wsaData;
SOCKET hSocket;
char message[20];
int strLen;
SOCKADDR_IN servAddr;
// Client

int NUM_FLAGS = 0;
int NUM_MINES = 0;
int LINE = 0;

long SCORE = 0;
int COMBO = 0;
int SQUARE = 0;

char** BOARD = NULL;
char** BACK = NULL;
char** serverBOARD = NULL;

point_t user = { 0, 0 };
mode_t mode;

int main()
{
	srand((unsigned)time(NULL));

	readme();

	CreateGame();
	InitiateGame();
	Mining();
	ShowBoard();

	while(PlayGame());

	/*
	get_score();
	show_score();
	*/

	FreeAll();

	return 0;
}