#include "ga.h"
#include <numeric>

extern int N;
extern int Psize;
extern double Dist[MAXN][MAXN];
extern SOL Population[MAXPSIZE];
extern SOL Record;
extern SOL WorstRec;

extern Parameter Params;

void print_stats(){
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

    fprintf(stderr, " mean: %f stdev: %f\n", mean, stdev);
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

const char* str(enum Represent e){
    switch (e){
    case OrderBase: return "OrderBase";
    case LocusBase: return "LocusBase";
    default: return "";
    }
}
const char* str(enum GenSolution e){
    switch (e)
    {
    case RandomGen: return "RandomGen";
    case DFS: return "DFS";
    default: return "";
    }
}
const char* str(enum Selection e){
    switch (e){
    case RandomSelect: return "RandomSelect";
    case Roulette: return "Roulette";
    case Tournament: return "Tournament";
    case Rank: return "Rank";
    case Sharing: return "Sharing";
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
    case Uniform: return "Uniform";
    case NonUniform: return "NonUniform";
    case Change: return "Change";
    default: return "";
    }
}
const char* str(enum Replacement e) {
    switch (e) {
    case Worst: return "Worst";
    case Preselection: return "Preselection";
    case Crowding: return "Crowding";
    default: return "";
    }
}
const char* str(enum Termination e) {
    switch (e) {
    case GenNumber: return "GenNumber"; 
    case Similarity: return "Similarity";
    default: return "";
    }
}

