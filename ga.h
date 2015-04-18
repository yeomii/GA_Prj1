#ifndef GA_H
#define GA_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAXN 318
#define MAXPSIZE 1000

typedef struct {
	int ch[MAXN];
	double f;	
} SOL;

double eval(SOL *s);

void gen_init_solution(SOL *s);

void selection(SOL **p1, SOL **p2);

void crossover(const SOL *p1, const SOL *p2, SOL *c);

void mutation(SOL *s);

void replacement(const SOL *offspr);

#endif
