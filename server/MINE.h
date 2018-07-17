#pragma once

#ifndef __STDLIB_H__
#define __STDLIB_H__
#include <stdlib.h>
#endif

typedef enum {single, multiple} menu_t;

typedef enum {flag, find} mode_t;

typedef struct point
{
	int x;
	int y;
} point_t;

void readme();

void Menu();

void CreateGame();
void InitiateGame();
void Mining();

void MultiSet();

void SendBoard();
void ReceiveBoard();

void ShowBoard();
void ShowBoardMulti();

void wider(int x, int y);

void SetFlag();
int CheckFlag();
int MineFound();

void Win();
void Lose();

int PlayGame();
int PlayGameMulti();

void FreeAll();