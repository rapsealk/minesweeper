#pragma once

#define MAX_NAME 100

typedef struct data {
	char name[MAX_NAME];
	long score;
	struct data *next;
} data_t;

data_t *create();
void combo_score();
void recur_score();
void print_score();
int get_score();
int show_score();