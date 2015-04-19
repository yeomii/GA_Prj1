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

int Q1idx, Q2idx, Q3idx;

void print_stats(){
    static int last = Generation;
    if (Generation - last < 10000)
        return;

    last = Generation;
    double sum = [](SOL* pop, int size) {
        double ret = 0.0;
        for (int i = 0; i < size; i++) {
            ret += pop[i].f;
        }
        return ret;
    }(Population, Psize);
    double mean = sum / Psize;

    double sq_sum = [](SOL* pop, int size) {
        double ret = 0.0;
        for (int i = 0; i < size; i++) {
            ret += pop[i].f * pop[i].f;
        }
        return ret;
    }(Population, Psize);
    double stdev = std::sqrt(sq_sum / Psize - mean * mean);

    fprintf(stderr, " min: %.2f mean: %.2f stdev: %.2f   Q1: %.2f Q2: %.2f Q3: %.2f\n", Record.f, mean, stdev, 
        Population[Q1idx].f, Population[Q2idx].f, Population[Q3idx].f);
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

void sort_population(){
    std::sort(Population, Population + Psize, [](SOL lhs, SOL rhs) {
        return lhs.f < rhs.f;
    });
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

