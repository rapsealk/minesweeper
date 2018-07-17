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

#ifndef __SCORE_H__
#define __SCORE_H__
#include "SCORE.h"
#endif

/*
#ifndef __SERVER_H__
#define __SERVER_H__
#include "server.h"
#endif
*/

// Server
WSADATA wsaData;
SOCKET hServSock;
SOCKET hClntSock;
SOCKADDR_IN servAddr;
SOCKADDR_IN clntAddr;
int szClntAddr;
char message[20];
// Server

int NUM_FLAGS = 0;
int NUM_MINES = 0;
int LINE = 0;

long SCORE = 0;
int COMBO = 0;
int SQUARE = 0;

char** BOARD = NULL;
char** BACK = NULL;
char** clientBOARD = NULL;

point_t user = { 0, 0 };
menu_t menu;
mode_t mode;

int main()
{
	srand((unsigned)time(NULL));

	readme();

	Menu();

	CreateGame();
	InitiateGame();
	Mining();
	if (menu == single) {
		ShowBoard();
		while (PlayGame());
	}
	else if (menu == multiple) {
		MultiSet();
		ShowBoardMulti();
		while (PlayGameMulti());
	}

	//get_score();
	//show_score();
	
	freesocket();
	FreeAll();

	return 0;
}