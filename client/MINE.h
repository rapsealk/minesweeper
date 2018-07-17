#pragma once

#ifndef __STDLIB_H__
#define __STDLIB_H__
#include <stdlib.h>
#endif

typedef enum {flag, find} mode_t;

typedef struct point
{
	int x;
	int y;
} point_t;

void readme();

void MultiSet();

void SendBoard();
void ReceiveBoard();

void CreateGame();
void InitiateGame();
void Mining();
void ShowBoard();

void wider(int x, int y);

void SetFlag();
int CheckFlag();
int MineFound();

void Win();
void Lose();

int PlayGame();
void FreeAll();