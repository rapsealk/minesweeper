#ifndef __STDIO_H__
#define __STDIO_H__
#include <stdio.h>
#endif

#ifndef __MINE_H__
#define __MINE_H__
#include "MINE.h"
#endif

#ifndef __GOTOXY_H__
#define __GOTOXY_H__
#include "GOTOXY.h"
#endif

#ifndef __SCORE_H__
#define __SCORE_H__
#include "SCORE.h"
#endif

#ifndef __WINSOCK2_H__
#define __WINSOCK2_H__
#include <WinSock2.h>
#endif

#ifndef __SERVER_H__
#define __SERVER_H__
#include "server.h"
#endif

extern int NUM_FLAGS;
extern int NUM_MINES;
extern int LINE;

extern char** BOARD;
extern char** BACK;
extern char** clientBOARD;

extern point_t user;
extern menu_t menu;
extern mode_t mode;

extern int COMBO;
extern int SQUARE;

extern WSADATA wsaData;
extern SOCKET hServSock;
extern SOCKET hClntSock;
extern SOCKADDR_IN servAddr;
extern SOCKADDR_IN clntAddr;
extern int szClntAddr;
extern char message[20];

void readme()
{
	puts("Minesweeper: Released on 2015/12/06 by rapsealk\n");

	puts("System:: readme");
	puts("1) Board - - - - - - - - - - - - - - - - - - - -");
	puts("#: 아직 선택되지 않은 공간입니다.");
	puts("NUM(0~8): 주변에 있는 지뢰의 수를 나타냅니다.");
	puts("M: 지뢰를 나타냅니다.");
	puts("F: 깃발을 나타냅니다.");
	puts("2) Mode - - - - - - - - - - - - - - - - - - - - ");
	puts("두 가지 모드가 제공됩니다.");
	puts("깃발을 꽂거나 제거하기 위해서는 '0 x y'의 형태로 입력하십시오.");
	puts("지뢰를 찾기 위해서는 '1 x y'의 형태로 입력하십시오.");
	puts("\n");
}

void Menu()
{
	printf("0.Single Play 1.MultiPlay\n");
	printf("Menu: ");
	scanf("%d", &menu);
	switch (menu) {
		case 0:
			break;
		case 1:
			// socket open
			opensocket();
			break;
		default:
			break;
	}
}

void CreateGame()
{
	int i;

	puts("System:: CreateGame");
	while (1) {
		printf("Input Board Size(N): ");
		if (scanf("%d", &LINE) == 1) {
			if (menu == multiple) {
				if (LINE > 10) {
					printf("Error:: Maximum for multiplay is 10\n");
					continue;
				}

				clientBOARD = malloc(sizeof(char*)*LINE);

				for (i = 0; i < LINE; i++) {
					clientBOARD[i] = malloc(sizeof(char)*LINE+1);
				}

			}
			if (LINE > 0) {
				printf("Board Size: %d X %d \n", LINE, LINE);
				break;
			}
		}
	}

	BOARD = malloc(sizeof(char*)*LINE);

	for (i = 0; i < LINE; i++) {
		BOARD[i] = malloc(sizeof(char)*(LINE+1));
	}

	BACK = malloc(sizeof(char*)*(LINE + 2));

	for (i = 0; i < LINE + 2; i++) {
		BACK[i] = malloc(sizeof(char)*(LINE + 2));
	}
	puts("System:: Game Created.\n");
}

void InitiateGame()
{
	int i, j;

	puts("System:: Initiate Game");
	for (i = 0; i < LINE; i++) {
		for (j = 0; j < LINE; j++) {
			BOARD[i][j] = '#';
		}
		BOARD[i][j] = '\0';
	}

	for (i = 0; i < LINE + 2; i++) {
		for (j = 0; j < LINE + 2; j++) {
			BACK[i][j] = '#';
		}
	}
	puts("System:: Game Initiated.\n");
}

void Mining()
{
	int count;
	int x, y;
	int i, j;

	puts("System:: Mining");
	printf("The number of mines: ");
	scanf("%d", &NUM_MINES);

	// Mining
	for (i = 0; i < NUM_MINES; ) {
		x = rand() % LINE + 1;
		y = rand() % LINE + 1;
		if (BACK[x][y] == '#') {
			BACK[x][y] = 'M';
			i++;
		}
	}

	for (x = 1; x < LINE + 1; x++) {
		for (y = 1; y < LINE + 1; y++) {
			count = 0;
			if (BACK[x][y] == '#') {
				for (i = x - 1; i <= x + 1; i++) {
					for (j = y - 1; j <= y + 1; j++) {
						if (BACK[i][j] == 'M')
							count++;
					}
				}
				BACK[x][y] = count + 48;
			}
		}
	}
	puts("System:: Mining Done.\n");
}

void MultiSet()
{
	int i, j;

	// Client에 LINE & NUM_MINES 전송
	sprintf(message, "%d %d", LINE, NUM_MINES);
	sendmessage(message);

	// Client Board
	for (i = 0; i < LINE; i++) {
		for (j = 0; j < LINE; j++) {
			clientBOARD[i][j] = '#';
		}
		clientBOARD[i][j] = '\0';
	}
}

void SendBoard()
{
	int i;

	for (i = 0; i < LINE; i++) {
		sprintf(message, "%s", BOARD[i]);
		sendmessage(message);
	}
}

void ReceiveBoard()
{
	int i, j;

	for (i = 0; i < LINE; i++) {
		receivemessage();
		for (j = 0; j < LINE; j++) {
			clientBOARD[i][j] = message[j];
		}
	}
}

void ShowBoard()
{
	int i;

	gotoxy(20, 13);
	print_score();

	for (i = 0; i < LINE; i++) {
		gotoxy(20, 14 + i);
		printf("%s\n", BOARD[i]);
	}
	puts("");
}

void ShowBoardMulti()
{
	int i;

	gotoxy(20, 13);
	print_score();

	for (i = 0; i < LINE; i++) {
		gotoxy(20, 14+i);
		printf("%s   %s\n", BOARD[i], clientBOARD[i]);
	}
	puts("");
}

void wider(int x, int y)
{
	int i, j;
	
	SQUARE++;

	for (i = x - 1; i <= x + 1; i++) {
		if (i < 0 || LINE-1 < i)
			continue;
		for (j = y - 1; j <= y + 1; j++) {
			if (j < 0 || LINE-1 < j)
				continue;
			if (BOARD[i][j] != '#')
				continue;
			if (BOARD[i][j] == 'F')
				continue;
			if (BACK[i + 1][j + 1] == 'M')
				continue;
			BOARD[i][j] = BACK[i + 1][j + 1];
			if (BOARD[i][j] == '0') {
				if (0 <= i && i <= LINE-1 && 0 <= j && j <= LINE-1)
					wider(i, j); // recur
			}
		}
	}
}

void SetFlag()
{
	if (BOARD[user.x][user.y] == '#') {
		BOARD[user.x][user.y] = 'F';
		if (BACK[user.x + 1][user.y + 1] == 'M') {
			NUM_FLAGS++;
		}
		else {
			NUM_FLAGS--;
		}
	}
	else if (BOARD[user.x][user.y] == 'F') {
		BOARD[user.x][user.y] = '#';
		if (BACK[user.x + 1][user.y + 1] == 'M') {
			NUM_FLAGS--;
		}
		else {
			NUM_FLAGS++;
		}
	}
}

int CheckFlag()
{
	if (NUM_FLAGS == NUM_MINES) {
		gotoxy(20, 18 + LINE);
		puts("Game Clear!");
		return 0;
	}
	return 1;
}

int MineFound()
{
	if (BOARD[user.x][user.y] == 'M') {
		gotoxy(20, 18 + LINE);
		puts("Game Over");
		return 1;
	}
	return 0;
}

void Win()
{
	gotoxy(20, 18 + LINE);
	puts("You Win!");
}

void Lose()
{
	gotoxy(20, 18 + LINE);
	puts("You Lost!");
}

int PlayGame()
{
	system("cls");
	readme();
	ShowBoard();
	gotoxy(20, 15+LINE);
	puts("Mode: 0.flag 1.find");
	while (1) {
		gotoxy(20, 16 + LINE);
		printf("(mode x y):                               ");
		gotoxy(32, 16 + LINE);
		scanf("%d %d %d", &mode, &user.x, &user.y);
		if ((mode == 0 || mode == 1) && (0 <= user.x && user.x <= LINE - 1) && (0 <= user.y && user.y <= LINE - 1)) {
			break;
		}
	}
	if (mode == flag) {
			SetFlag();
			ShowBoard();
			if (!CheckFlag()) {
				puts("");
				return 0;
			}
	}
	else if (mode == find) {
		if (BOARD[user.x][user.y] == '#') {
			BOARD[user.x][user.y] = BACK[user.x + 1][user.y + 1];
			if (BOARD[user.x][user.y] == '0') {
				SQUARE = 0;
				wider(user.x, user.y);
				recur_score();
				ShowBoard();
			}
			else if (MineFound()) {
				ShowBoard();
				puts("");
				return 0;
			}
			else {
				combo_score();
				ShowBoard();
			}
		}
	}
	return 1;
}

int PlayGameMulti() // sendboard -> sendmessage -> receiveboard -> receivemessage
{
	system("cls");
	readme();
	ShowBoardMulti();
	gotoxy(20, 15 + LINE);
	puts("Mode: 0.flag 1.find");
	while (1) {
		gotoxy(20, 16 + LINE);
		printf("(mode x y):                               ");
		gotoxy(32, 16 + LINE);
		scanf("%d %d %d", &mode, &user.x, &user.y);
		if ((mode == 0 || mode == 1) && (0 <= user.x && user.x <= LINE - 1) && (0 <= user.y && user.y <= LINE - 1)) {
			break;
		}
	}
	if (mode == flag) {
		// Server Turn
		SetFlag();
		SendBoard();
		ShowBoardMulti();
		if (!CheckFlag()) {
			puts("");
			Win();
			sendmessage("WIN");
			return 0;
		}
		else {
			sendmessage("NOTHING");
		}
		gotoxy(20, 15 + LINE);
		puts("Waiting for the Client");
		gotoxy(20, 16 + LINE);
		puts("Network Minesweeper coded by rapsealk");
		// Client Turn
		ReceiveBoard();
		ShowBoardMulti();
		receivemessage();
	}
	else if (mode == find) {
		if (BOARD[user.x][user.y] == '#') {
			BOARD[user.x][user.y] = BACK[user.x + 1][user.y + 1];
			if (BOARD[user.x][user.y] == '0') {
				SQUARE = 0;
				wider(user.x, user.y);
				recur_score();
			}
			else if (MineFound()) {
				SendBoard();
				ShowBoardMulti();
				Lose();
				sendmessage("LOST");
				puts("");
				return 0;
			}
			else {
				combo_score();
			}
			SendBoard();
			ShowBoardMulti();
			sendmessage("NOTHING");
			gotoxy(20, 15 + LINE);
			puts("Waiting for the Client");
			gotoxy(20, 16 + LINE);
			puts("Network Minesweeper coded by rapsealk");
			// Client Turn
			ReceiveBoard();
			ShowBoardMulti();
			receivemessage();
		}
	}
	if (!strcmp(message, "LOST")) {
		Win();
		return 0;
	}
	else if (!strcmp(message, "WIN")) {
		Lose();
		return 0;
	}
	return 1;
}

void FreeAll()
{
	int i;

	gotoxy(0, 20 + LINE);

	puts("System:: Free Heap Memories");

	for (i = 0; i < LINE; i++) {
		free(BOARD[i]);
	}
	free(BOARD);

	for (i = 0; i < LINE + 2; i++) {
		free(BACK[i]);
	}
	free(BACK);

	if (menu == multiple) {
		for (i = 0; i < LINE; i++) {
			free(clientBOARD[i]);
		}
		free(clientBOARD);
	}

	puts("System:: Successfully Freed.\n");
}