#ifndef GA_H
#define GA_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string>
using namespace std;

#define MAXN 318
#define MAXPSIZE 1000

enum Represent { OrderBase, LocusBase };
enum GenSolution { RandomGen, DFS };
enum Selection { RandomSelect, Roulette, Tournament, Rank, Sharing };
enum Crossover { Cycle, Order, PMX, EdgeRecombination };
enum Mutation { Uniform, NonUniform, Change };
enum Replacement { Worst, Preselection, Crowding };
enum Termination { GenNumber, Similarity };

typedef struct {
	int ch[MAXN];
	double f;	
} SOL;

typedef struct {
    Represent represent;

    GenSolution genSolution;

    Selection selection;
    int roulette_k;
    int tournament_k;
    double tournament_t;
    double rank_max;
    double rank_min;

    Crossover crossover;
    Mutation mutation;
    Replacement replacement;

    Termination termination;
} Parameter;

double eval(SOL *s);

const char* str(enum Represent e);
const char* str(enum GenSolution e);
const char* str(enum Selection e);
const char* str(enum Crossover e);
const char* str(enum Mutation e);
const char* str(enum Replacement e);
const char* str(enum Termination e);

void gen_init_solution(SOL *s);

void selection(SOL **p1, SOL **p2);

void crossover(const SOL *p1, const SOL *p2, SOL *c);

void mutation(SOL *s);

void replacement(const SOL *offspr);

#endif
