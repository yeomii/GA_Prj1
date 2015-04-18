#include "ga.h"

extern int Psize;
extern SOL Population[MAXPSIZE];
extern SOL Record;
extern SOL WorstRec;
extern Parameter Params;

double roulette_fitness[MAXPSIZE];

void random_selection(SOL **p1, SOL **p2);
void roulette_selection(SOL **p1, SOL **p2);

void selection(SOL **p1, SOL **p2){
    switch (Params.selection){
    case RandomSelect:
        random_selection(p1, p2);
        break;
    case Roulette:
        roulette_selection(p1, p2);
        break;
    }
}

void random_selection(SOL **p1, SOL **p2){
    *p1 = &Population[rand() % Psize];
    *p2 = &Population[rand() % Psize];
}

int roulette_calc(double fit_sum, SOL **p){
    for (int i = 0; i < Psize; i++){
        int point = rand() % (int)(fit_sum);
        int rand_sum = 0;
        for (int i = 0; i < Psize; i++){
            rand_sum += roulette_fitness[i];
            if (rand_sum > point){
                *p = &Population[i];
                return i;
            }
        }
    }
    *p = &Population[Psize-1];
    return Psize - 1;
}

void roulette_selection(SOL **p1, SOL **p2){
    // calc fitness
    double fit_sum = 0;
    for (int i = 0; i < Psize; i++){
        roulette_fitness[i] = (WorstRec.f - Population[i].f) +
            ((WorstRec.f - Record.f) / (Params.roulette_k - 1));
        fit_sum += roulette_fitness[i];
    }
    int idx1 = roulette_calc(fit_sum, p1);
    int idx2 = roulette_calc(fit_sum, p2);

    //fprintf(stderr, "%f %f %f %f\n", 
    //  roulette_fitness[idx1] /fit_sum, roulette_fitness[idx2]/fit_sum, Record.f, WorstRec.f);
}