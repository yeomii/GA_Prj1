#include "ga.h"
#include <numeric>
#include <algorithm>

extern int N;
extern int Psize;
extern int Generation;
extern double Dist[MAXN][MAXN];
extern SOL Population[MAXPSIZE];
extern SOL Record;
extern SOL WorstRec;

extern Parameter Params;

void print_stats(FILE *file){
    if (Generation % Params.print_freq != 0)
        return;

    sort_population();

    double sum = 0.0;
    for (int i = 0; i < Psize; i++) {
        sum += Population[i].f;
    }
    double mean = sum / Psize;

    double sq_sum = 0.0;
    for (int i = 0; i < Psize; i++) {
        sq_sum += Population[i].f * Population[i].f;
    }
    double stdev = sqrt(sq_sum / Psize - mean * mean);

    fprintf(file, "gen:%d q0:%.2f q1:%.2f q2:%.2f q3:%.2f q4:%.2f\navg:%.2f stdev:%.2f\n", 
        Generation,
        Population[0].f, Population[Psize/4].f, Population[Psize/2].f, Population[3*Psize/4].f, Population[Psize-1].f,
        mean, stdev);
}

void print_parameters(FILE *file){
    string fs;
    fs.append("Represent : %s\n")
        .append("Selection : %s\n")
        .append("Crossover : %s\n")
        .append("Mutation : %s\n")
        .append("Replacement : %s\n")
        .append("Termination : %s\n");
    fprintf(file, fs.c_str(),
        str(Params.represent), str(Params.selection), str(Params.crossover),
        str(Params.mutation), str(Params.replacement), str(Params.termination));

    fs.clear();
    fs.append("population_size : %d\n")
        .append("generation : %d\n")
        .append("generation gap : %f\n")
        .append("roulette_k : %d\n")
        .append("tournament_k : %d\t tournament_t : %f\n")
        .append("rank_max : %f\t rank_min : %f\n")
        .append("mutation_t : %f\t mutation_b : %f\n");
    fprintf(file, fs.c_str(), Psize, Generation, Params.generation_gap, Params.roulette_k, 
        Params.tournament_k, Params.tournament_t, Params.rank_max, Params.rank_min, 
        Params.mutation_t, Params.mutation_b);
}

void parse_parameters(FILE *file) {
    fscanf(file, "s:%d c:%d m:%d r:%d rk:%d tk:%d tt:%lf rM:%lf rm:%lf mt:%lf mb:%lf gg:%lf pf:%d ps:%d",
        &Params.selection,
        &Params.crossover,
        &Params.mutation,
        &Params.replacement,
        &Params.roulette_k,
        &Params.tournament_k,
        &Params.tournament_t,
        &Params.rank_max,
        &Params.rank_min,
        &Params.mutation_t,
        &Params.mutation_b,
        &Params.generation_gap,
        &Params.print_freq,
        &Psize);
}

// calculate the fitness of s and store it into s->f
double eval(SOL *s) {
	s->f = 0;
    if (Params.represent == OrderBase){
        for (int i = 0; i < N; i++) {
            s->f += Dist[s->ch[i]][s->ch[(i + 1) % N]];
        }
    }
    else{
        for (int i = 0; i<N; i++){
            s->f += Dist[i][s->ch[i]];
        }
    }

	// if best, renew record
	if (Record.f > s->f) {
		Record.f = s->f;
		for (int i = 0; i < N; i++) {
			Record.ch[i] = s->ch[i];
		}
    }
    else if (WorstRec.f < s->f){
        WorstRec.f = s->f;
        for (int i = 0; i < N; i++) {
            WorstRec.ch[i] = s->ch[i];
        }
    }

	return s->f;
}

bool sol_comp(SOL lhs, SOL rhs) {
    return lhs.f < rhs.f;
}

void sort_population(){
    std::sort(Population, Population + Psize, sol_comp);
}

const char* str(enum Represent e){
    switch (e){
    case OrderBase: return "OrderBase";
    case LocusBase: return "LocusBase";
    default: return "";
    }
}
const char* str(enum Selection e){
    switch (e){
    case RandomSelect: return "RandomSelect";
    case Roulette: return "Roulette";
    case Tournament: return "Tournament";
    case Rank: return "Rank";
    default: return "";
    }
}
const char* str(enum Crossover e){
    switch (e)
    {
    case Cycle: return "Cycle";
    case Order: return "Order";
    case PMX: return "PMX";
    case EdgeRecombination: return "EdgeRecombination";
    default: return "";
    }
}
const char* str(enum Mutation e) {
    switch (e) {
    case ChangeMix: return "ChangeMix";
    case ChangeTwo: return "ChangeTwo";
    case ChangeOr: return "ChangeOr";
    case ChangeSwap: return "ChangeSwap";
    default: return "";
    }
}
const char* str(enum Replacement e) {
    switch (e) {
    case Random: return "Random";
    case Worst: return "Worst";
    case Preselection: return "Preselection";
    default: return "";
    }
}
const char* str(enum Termination e) {
    switch (e) {
    case LimitTime: return "LimitTime";
    case GenNumber: return "GenNumber"; 
    case Similarity: return "Similarity";
    default: return "";
    }
}

