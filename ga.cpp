#include "ga.h"
#include <chrono>
#include <thread>

int N;
int Psize = 50;
double X[MAXN], Y[MAXN];
double Dist[MAXN][MAXN];
int Near[MAXN];

SOL Population[MAXPSIZE];
SOL Record;
SOL WorstRec;

Parameter Params = {};
int Generation = 0;

extern int Q1idx, Q2idx, Q3idx;

// Time limit for the test case
long long TimeLimit;
time_t BeginTime;

void print_sol(SOL *s, FILE* file) {
    for (int i = 0; i < N; i++) {
        if (i > 0) fprintf(file, " ");
        fprintf(file, "%d", s->ch[i] + 1);
    }
    fprintf(file, " ----- %f\n", s->f);
}

// a "steady-state" GA
void GA() {
	int i;
	SOL c;
	BeginTime = time(NULL);

	for (i = 0; i < Psize; i++) {
		gen_init_solution(&Population[i]);
	}

	while (1) {
		if(time(NULL) - BeginTime >= TimeLimit - 1) return; // end condition
        sort_population();
		SOL *p1, *p2;
		selection(&p1, &p2);
		crossover(p1, p2, &c);
        mutation(&c);
        normalize_solution(&c);
		replacement(&c);
		Generation++;
        print_stats();
        //std::this_thread::sleep_for(std::chrono::milliseconds(50));
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
    WorstRec.f = -1.0;
    Q1idx = (int)Psize / 4;
    Q2idx = Q1idx + Q1idx;
    Q3idx = Q2idx + Q1idx;
}

// print the best solution found to stdout
void answer() {
    if (Params.represent == LocusBase){
        // change to order-base
        int *tmp = new int[N];
        int visit = 0;
        memcpy(tmp, Record.ch, N * sizeof(int));
        for (int i = 0; i < N; i++){
            Record.ch[i] = visit;
            visit = tmp[visit];
        }
        delete[] tmp;
    }
    print_sol(&Record, stdout);
}

void init_params(int argc, char *argv[]){
    //Params.represent = LocusBase;

    Params.selection = Tournament;
    Params.tournament_k = 5;
    Params.tournament_t = 0.7;
    
    Params.crossover = PMX;
    
    Params.mutation = ChangeMix;
    Params.mutation_t = 0.05;
    Params.mutation_b = 5;

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

