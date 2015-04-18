#include "ga.h"

extern int N;

// combine the given parents p1 and p2
// and store the generated solution at c
// currently the child will be same as p1
void default_crossover(const SOL *p1, const SOL *p2, SOL *c);

void crossover(const SOL *p1, const SOL *p2, SOL *c) {
	default_crossover(p1, p2, c);
	eval(c);
}

void default_crossover(const SOL *p1, const SOL *p2, SOL *c){
	int i;
	for (i = 0; i < N; i++) {
		c->ch[i] = p1->ch[i];
	}
}
