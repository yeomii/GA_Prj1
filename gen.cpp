#include "ga.h"

extern int N;

void gen_rand_solution(SOL *s);

void gen_init_solution(SOL *s){
	gen_rand_solution(s);
	eval(s);
}

// generate a random order-based solution at s
void gen_rand_solution(SOL *s){
	int i;
	for (i = 0; i < N; i++) {
		s->ch[i] = i;
	}
	for (i = 0; i < N; i++) {
		int r = i + rand() % (N - i);	// r is a random number in [i..N-i)
		int tmp = s->ch[i]; s->ch[i] = s->ch[r]; s->ch[r] = tmp; // swap
	}
}
