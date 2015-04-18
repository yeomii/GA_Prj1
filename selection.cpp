#include "ga.h"

extern int Psize;
extern SOL Population[MAXPSIZE];

void random_selection(SOL **p1, SOL **p2);

void selection(SOL **p1, SOL **p2){
	random_selection(p1, p2);
}


void random_selection(SOL **p1, SOL **p2){
	*p1 = &Population[rand() % Psize];
	*p2 = &Population[rand() % Psize];
}
