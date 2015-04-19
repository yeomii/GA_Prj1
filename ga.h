#ifndef GA_H
#define GA_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <string>
using namespace std;

#define MAXN 318
#define MAXPSIZE 1000

enum Represent { OrderBase = 0, LocusBase = 1 };
enum Selection { RandomSelect = 0, Roulette = 1, Tournament = 2, Rank = 3 };
enum Crossover { Cycle = 0, Order = 1, PMX = 2, EdgeRecombination = 3 };
enum Mutation { ChangeTwo = 0, ChangeOr = 1, ChangeSwap = 2, ChangeMix = 3 };
enum Replacement { Random = 0, Worst = 1, Preselection = 2 };
enum Termination { LimitTime, GenNumber, Similarity };

typedef struct {
	int ch[MAXN];
	double f;	
} SOL;

typedef struct {
    Represent represent;

    Selection selection;
    int roulette_k;
    int tournament_k;
    double tournament_t;
    double rank_max;
    double rank_min;

    Crossover crossover;
    
    Mutation mutation;
    double mutation_t; // threshold
    double mutation_b; // if larger than 1, non-uniform

    Replacement replacement;
    double generation_gap; // if 0, steady-state GA and 1, generational GA 

    Termination termination;

    int print_freq;
} Parameter;

void print_stats(FILE *file);
void print_parameters(FILE *file);
void parse_parameters(FILE *file);
double eval(SOL *s);
void sort_population();

const char* str(enum Represent e);
const char* str(enum Selection e);
const char* str(enum Crossover e);
const char* str(enum Mutation e);
const char* str(enum Replacement e);
const char* str(enum Termination e);

void gen_init_solution(SOL *s);
void normalize_solution(SOL *s);

void selection(SOL **p1, SOL **p2);

void crossover(const SOL *p1, const SOL *p2, SOL *c);

void mutation(SOL *s);

void replacement(SOL *p1, SOL *p2, const SOL *offspr);
void worst_generational_replacement(SOL* offsprs, int size);
#endif
