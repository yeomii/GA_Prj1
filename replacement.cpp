#include "ga.h"

extern int N;
extern int Psize;
extern SOL Population[MAXPSIZE];

void random_replacement(const SOL *offspr);

void replacement(const SOL *offspr){
	random_replacement(offspr);
}

// replace one solution from the population with the new offspring
// currently any random solution can be replaced
void random_replacement(const SOL *offspr){
	int i, p = rand() % Psize;
	Population[p].f = offspr->f;
	for (i = 0; i < N; i++) {
		Population[p].ch[i] = offspr->ch[i];
	}
}
