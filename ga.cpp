#include "ga.h"
#include <chrono>
#include <thread>

int N;
int Psize;
double X[MAXN], Y[MAXN];
double Dist[MAXN][MAXN];

SOL Population[MAXPSIZE];
SOL Record;
SOL WorstRec;

Parameter Params = {};
int Generation = 0;

// Time limit for the test case
long long TimeLimit;
time_t BeginTime;
FILE* sf;

void print_sol(SOL *s, FILE* file) {
    for (int i = 0; i < N; i++) {
        if (i > 0) fprintf(file, " ");
        fprintf(file, "%d", s->ch[i] + 1);
    }
    fprintf(file, "\n%f\n", s->f);
    print_parameters(file);
}

// a "steady-state" GA
void GA() {
    int new_offspr = (int) Psize * Params.generation_gap;
    SOL *offsprs = new SOL[Psize];

	for (int i = 0; i < Psize; i++) {
		gen_init_solution(&Population[i]);
	}

    while (1){
        if (time(NULL) - BeginTime >= TimeLimit - 1) return; // end condition

        sort_population();
        if (new_offspr == 0){
            SOL c;
            SOL *p1, *p2;
            selection(&p1, &p2);
            crossover(p1, p2, &c);
            mutation(&c);
            normalize_solution(&c);
            replacement(p1, p2, &c);

        }
        else{
            for (int i = 0; i < new_offspr; i++)
            {
                if (time(NULL) - BeginTime >= TimeLimit - 1) return; // end condition
                SOL *c = &offsprs[i];
                SOL *p1, *p2;
                selection(&p1, &p2);
                crossover(p1, p2, c);
                mutation(c);
                normalize_solution(c);
            }
            if (Params.replacement == Worst){
                worst_generational_replacement(offsprs, new_offspr);
            }
            else{
                for (int i = 0; i < new_offspr; i++){
                    replacement(NULL, NULL, &offsprs[i]);
                }
            }
        }
        Generation++;
        print_stats(sf);
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
    BeginTime = time(NULL);

	Record.f = 1e100;
    WorstRec.f = -1.0;
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

int main(int argc, char* argv[]) {
	srand(time(NULL));

  long long x = time(NULL);
  char out[256], stat[256];

  if (argc == 1) return 0;

  freopen("cycle.in", "r", stdin);
	
	init();
  sprintf(out, "result_%lld.%d.out", x, N);
  sprintf(stat, "trial_%lld.%d.out", x, N);
  //
  freopen(out, "w", stdout);
  FILE* cf = fopen(argv[1], "r");
  sf = fopen(stat, "w");
  //
  parse_parameters(cf);
  print_parameters(sf);
	GA();
	answer();
	return 0;
}

