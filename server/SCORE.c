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

#ifndef __SCORE_H__
#define __SCORE_H__
#include "SCORE.h"
#endif

#ifndef __GOTOXY_H__
#define __GOTOXY_H__
#include "GOTOXY.h"
#endif

extern int LINE;

extern long SCORE;
extern int COMBO;
extern int SQUARE;

data_t *create()
{
	data_t *temp;
	temp = malloc(sizeof(data_t));
	return temp;
}

void combo_score()
{
	SCORE += 10 * ++COMBO;
}

void recur_score()
{
	SCORE += 10 * SQUARE * SQUARE;
}

void print_score()
{
	printf("score: %06li \n", SCORE);
}

int get_score()
{
	data_t user;
	data_t a = { "ABC", 0, NULL };
	data_t b = { "DEF", 0, NULL };
	data_t c = { "GHI", 0, NULL };
	char temp;
	int len = 0;
	FILE *fp = fopen("score.bin", "wb");
	
	if (fp == NULL) {
		fprintf(stderr, "Failed to open file.\n");
		return -1;
	}

	user.score = SCORE;
	
	fgetc(stdin); // '\n'
	gotoxy(30 + LINE, 13);
	printf("Player score: %li\n", user.score);
	gotoxy(30 + LINE, 14);
	printf("Input Player Name: ");
	fgets(user.name, MAX_NAME, stdin);
	user.name[strlen(user.name) - 1] = '\0';
	user.next = NULL;

	//fseek(fp, 0, SEEK_END);
	fwrite((void*)&a, sizeof(data_t), 1, fp);
	fwrite((void*)&b, sizeof(data_t), 1, fp);
	fwrite((void*)&c, sizeof(data_t), 1, fp);
	fwrite((void*)&user, sizeof(data_t), 1, fp);
	
	fclose(fp);

	return 0;
}

int show_score()
{
	FILE *fp = fopen("score.bin", "rb");
	data_t *head = NULL;
	data_t *tail = NULL;
	data_t *st = NULL;
	data_t *next = NULL;
	data_t *temp = NULL;
	data_t box;
	int i = 0;

	if (fp == NULL) {
		fprintf(stderr, "Failed to open file.\n");
		return -1;
	}

	// Insertion Sort
	head = tail = create();
	fread((void*)head, sizeof(data_t), 1, fp);

	while (fread((void*)&box, sizeof(data_t), 1, fp)) {
		temp = create();
		*temp = box;
		if (temp->score > head->score) {
			temp->next = head;
			head = temp;
			continue;
		}
		else if (tail->score > temp->score) {
			tail = tail->next = temp;
			continue;
		}
		else {
			for (st = head; st != tail; st = st->next) {
				if (st->score >= temp->score && temp->score >= st->next->score) {
					temp->next = st->next;
					st->next = temp;
					break;
				}
			}
		}
	}

	gotoxy(30 + LINE, 16);
	puts("-Rank- -Name- -Score-");
	for (st = head; st != NULL; st = st->next) {
		gotoxy(30 + LINE, 16 + (++i));
		printf("%d) %s %06li \n", i, st->name, st->score);
		if (i > 5) {
			break;
		}
	}
	for (st = head; st != NULL; st = next) {
		next = st->next;
		free(st);
	}

	return 0;
}