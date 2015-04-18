#include "ga.h"

int N;
int Psize = 100;
double X[MAXN], Y[MAXN];
double Dist[MAXN][MAXN];
int Near[MAXN];

SOL Population[MAXPSIZE];
SOL Record;

Parameter Params = {};
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

void init_params(int argc, char *argv[]){
    if (argc == 1) return;
    for (int i = 1; i < argc; i++){
        fprintf(stderr, "%s\n", argv[i]);
    }
}

void print_parameters(){
    string fs;
    fs.append("Represent : %s\n")
        .append("GenSolution : %s\n")
        .append("Selection : %s\n")
        .append("Crossover : %s\n")
        .append("Mutation : %s\n")
        .append("Replacement : %s\n")
        .append("Termination : %s\n");
    fprintf(stderr, fs.c_str(),
        str(Params.represent), str(Params.genSolution), str(Params.selection),
        str(Params.crossover), str(Params.mutation), str(Params.replacement), str(Params.termination));
}

int main(int argc, char* argv[]) {
	srand(time(NULL));
    //
    freopen("cycle.in", "r", stdin);
    freopen("cycle.out", "w", stdout);
    //
    fprintf(stderr, "executing init with cycle.in...\n");
	init();
    fprintf(stderr, "initializing GA parameters...\n");
    init_params(argc, argv);
    print_parameters();
    fprintf(stderr, "starting GA...\n");
	GA();
    fprintf(stderr, "done\n");
	answer();
	return 0;
}

