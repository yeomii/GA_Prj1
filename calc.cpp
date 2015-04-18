#include "ga.h"

extern int N;
extern double Dist[MAXN][MAXN];
extern SOL Record;

// calculate the fitness of s and store it into s->f
double eval(SOL *s) {
	int i;

	s->f = 0;
	for (i = 0; i < N; i++) {
		s->f += Dist[s->ch[i]][s->ch[(i+1)%N]];
	}

	// if best, renew record
	if (Record.f > s->f) {
		Record.f = s->f;
		for (i = 0; i < N; i++) {
			Record.ch[i] = s->ch[i];
		}
	}

	return s->f;
}
