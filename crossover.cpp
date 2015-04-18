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

void cycle_crossover(const SOL *p1, const SOL *p2, SOL *c) {
	memset(c->ch, -1, N * sizeof(int));

	int *pos1 = new int[N], *pos2 = new int[N];
	for (int i = 0; i < N; i++) {
		pos1[p1->ch[i]] = i;
		pos2[p2->ch[i]] = i;
	}

	for (int i = 0; i < N; i++) {
		if (c->ch[i] != -1)
			continue;
		swap(p1, p2);
		swap(pos1, pos2);
		int j = i;
		while (1) {
			c->ch[j] = p2->ch[j];
			j = pos2[p1->ch[j]];
			if (i == j) break;
		}
	}

	delete[] pos1;
	delete[] pos2;

	// TODO: locus representation의 경우 하나의 cycle이 되도록 repair 해 주어야 한다.
}

void order_crossover(const SOL *p1, const SOL* p2, SOL *c) {
	int left = rand() % N, right = rand() % N;
	if (left > right) {
		swap(left, right);
		swap(p1, p2);
	}

	int *has = new int[N];
	for (int i = left; i < right; i++) {
		c->ch[i] = p1->ch[i];
		has[p1->ch[i]] = true;
	}
	int i = 0;
	for (int j = right; j != left; i = (i+1) % N) {
		if (has[p2->ch[i]]) continue;
		c->ch[j++] = p2->ch[i];
	}

	delete[] has;
}

void pmx_crossover(const SOL *p1, const SOL* p2, SOL *c) {
	int left = rand() % N, right = rand() % N;
	if (left > right) {
		swap(left, right);
		swap(p1, p2);
	}

	int *pos = new int[N];
	memset(pos, -1, N * sizeof(int));
	for (int i = left; i < right; i++) {
		c->ch[i] = p1->ch[i];
		pos[p1->ch[i]] = i;
	}
	for (int i = right; i != left; i = (i+1) % N) {
		int val = p2->ch[i];
		while (pos[val] != -1) {
			int next_pos = pos[p2->ch[i]];
			val = p1->ch[next_pos];
		}
		c->ch[i] = val;
	}

	delete[] pos;
}