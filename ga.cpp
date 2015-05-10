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
  int new_offspr = (int)Psize * Params.generation_gap;
  SOL *offsprs = new SOL[Psize];

  for (int i = 0; i < Psize; i++) {
    gen_init_solution(&Population[i]);
  }

  sort_population();

  while (1){
    if (time(NULL) - BeginTime >= TimeLimit - 1) return; // end condition

    if (new_offspr == 0){
      SOL c;
      SOL *p1, *p2;
      selection(&p1, &p2);
      crossover(p1, p2, &c);
      mutation(&c);
      two_opt(&c);
      eval(&c);
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
        two_opt(c);
        eval(c);
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

void two_opt_experiment(){
  for (int i = 0; i < Psize; i++)
    gen_init_solution(&Population[i]);

  Generation = Params.print_freq;
  print_stats(sf);

  for (int i = 0; i < Psize; i++) {
    SOL *c = &Population[i];
    eval(c);
    two_opt(c);
    eval(c);
  }
  print_stats(sf);
  print_sol(&Record, sf);
}

void multistart_two_opt() {
  for (int i = 0; i < Params.execution_n; i++) {
    SOL c;
    gen_init_solution(&c);
    two_opt(&c);
    eval(&c);
    if (i % Params.print_freq == 0)
      fprintf(sf, "%d gen -- %.2f\n", i, Record.f);
  }
  print_sol(&Record, stdout);
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
  TimeLimit = (long long)time_limit;
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

void init_parameters(){
  Params.execution = MultistartTwoOpt;
  Params.execution_n = 100000;
  Params.selection = Tournament;
  Params.crossover = PMX;
  Params.mutation = ChangeMix;
  Params.replacement = Preselection;
  Params.roulette_k = 5;
  Params.tournament_k = 3;
  Params.tournament_t = 0.8;//0.9
  Params.rank_max = 100.0;
  Params.rank_min = 1.0;
  Params.mutation_t = 0.1;
  Params.mutation_b = 4;//3
  Params.generation_gap = 0;
  Params.print_freq = 100;
  Psize = 100;
}

int parsing(int argc, char* argv[]){
	if (argc < 4){
		printf("Usage : ./ga input config number");
		return -1;
	}
	int n = atoi(argv[3]);
	char out[256], stat[256];
	sprintf(out, "result_%d.out", n);
	sprintf(stat, "stat_%d.out", n);

	FILE *cf = fopen(argv[2], "r");
	parse_parameters(cf);
	fclose(cf);

	freopen(argv[1], "r", stdin);
	freopen(out, "w", stdout);
	sf = fopen(stat, "w");
	return 0;
}

int main(int argc, char* argv[]) {
  srand(time(NULL));

	//
  if (parsing(argc, argv) < 0)
		return 0;
	//
  init();
  //init_parameters();
  //
	printf("N : %d\n", N);
	fprintf(sf, "N : %d\n", N);
	//
	
  switch (Params.execution){
  case TwoOpt:
    two_opt_experiment();
    break;
  case MultistartTwoOpt:
    multistart_two_opt();
    break;
  case HybridGA:
    GA();
    answer();
    break;
  }

  return 0;
}

