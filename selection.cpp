#include "ga.h"
#include <algorithm>

extern int Psize;
extern SOL Population[MAXPSIZE];
extern SOL Record;
extern SOL WorstRec;
extern Parameter Params;

double roulette_fitness[MAXPSIZE];


void random_selection(SOL **p1, SOL **p2);
void roulette_selection(SOL **p1, SOL **p2);
void tournament_selection(SOL **p1, SOL **p2);
void rank_selection(SOL **p1, SOL **p2);

void selection(SOL **p1, SOL **p2){
    switch (Params.selection){
    case RandomSelect:
        random_selection(p1, p2);
        break;
    case Roulette:
        roulette_selection(p1, p2);
        break;
    case Tournament:
        tournament_selection(p1, p2);
        break;
    case Rank:
        rank_selection(p1, p2);
        break;
    }
}

void random_selection(SOL **p1, SOL **p2){
    *p1 = &Population[rand() % Psize];
    *p2 = &Population[rand() % Psize];
}

void roulette_calc(double fit_sum, SOL **p){
    for (int i = 0; i < Psize; i++){
        int point = rand() % (int)(fit_sum);
        int rand_sum = 0;
        for (int i = 0; i < Psize; i++){
            rand_sum += roulette_fitness[i];
            if (rand_sum > point){
                *p = &Population[i];
                return;
            }
        }
    }
    *p = &Population[Psize-1];
}

void roulette_selection(SOL **p1, SOL **p2){
    // calc fitness
    double fit_sum = 0;
    for (int i = 0; i < Psize; i++){
        roulette_fitness[i] = (WorstRec.f - Population[i].f) +
            ((WorstRec.f - Record.f) / (Params.roulette_k - 1));
        fit_sum += roulette_fitness[i];
    }
    roulette_calc(fit_sum, p1);
    roulette_calc(fit_sum, p2);

    //fprintf(stderr, "%f %f %f %f\n", 
    //  roulette_fitness[idx1] /fit_sum, roulette_fitness[idx2]/fit_sum, Record.f, WorstRec.f);
}

void tournament_selection(SOL **p1, SOL **p2){
    int candidates = pow(2, Params.tournament_k);
    int *selected = new int[candidates];
    
    for (int i = 0; i < candidates; i++){
        selected[i] = rand() % Psize;
    }

    
    for (int step = 0; step < Params.tournament_k - 1; step++){
        candidates /= 2;
        for (int i = 0; i < candidates; i++){
            double f1 = Population[selected[2 * i]].f;
            double f2 = Population[selected[2 * i + 1]].f;
            double r = ((double)rand() / (RAND_MAX));
            if (r < Params.tournament_t)
                selected[i] = (f1 < f2) ? selected[2 * i] : selected[2 * i + 1];
            else
                selected[i] = (f1 < f2) ? selected[2 * i + 1] : selected[2 * i];
        }
    }

    *p1 = &Population[selected[0]];
    *p2 = &Population[selected[1]];

    delete[] selected;
}

void rank_selection(SOL **p1, SOL **p2){
		sort_population();

    double fit_sum = 0;
    for (int i = 0; i < Psize; i++){
        roulette_fitness[i] = Params.rank_max + 
            i * (Params.rank_min - Params.rank_max) / (Psize - 1);
        fit_sum += roulette_fitness[i];
    }
    roulette_calc(fit_sum, p1);
    roulette_calc(fit_sum, p2);
}
