#include "ga.h"

int N;
int Psize = 100;
double X[MAXN], Y[MAXN];
double Dist[MAXN][MAXN];
SOL Population[MAXPSIZE];
SOL Record;
int Generation = 0;

// Time limit for the test case
long long TimeLimit;

// a "steady-state" GA
void GA() {
	int i;
	SOL c;
	time_t begin = time(NULL);

	for (i = 0; i < Psize; i++) {
		gen_init_solution(&Population[i]);
	}

	while (1) {
		if(time(NULL) - begin >= TimeLimit - 1) return; // end condition
		SOL *p1, *p2;
		selection(&p1, &p2);
		crossover(p1, p2, &c);
		mutation(&c);
		replacement(&c);
		Generation++;
	}
}


// read the test case from stdin
// and initialize some values such as record.f and Dist
void init() {
	int i, j, tmp;
	double time_limit;

	tmp = scanf("%d", &N);
	for (i = 0; i < N; i++) {
		tmp = scanf("%lf %lf", &X[i], &Y[i]);
	}
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			double dx = X[i] - X[j], dy = Y[i] - Y[j];
			Dist[i][j] = sqrt(dx*dx + dy*dy);
		}
	}
	tmp = scanf("%lf", &time_limit);
	TimeLimit = (long long) time_limit;

	Record.f = 1e100;
}


// print the best solution found to stdout
void answer() {
	int i;

	for (i = 0; i < N; i++) {
		if (i > 0) printf(" ");
		printf("%d", Record.ch[i]+1);
	}
	printf("\n");
}


int main(int argc, char* argv[]) {
	srand(time(NULL));

	init();
	GA();
	answer();
	return 0;
}

