#include "ga.h"

extern int N;
extern int Psize;
extern SOL Population[MAXPSIZE];

extern Parameter Params;

void random_replacement(const SOL *offspr);
void worst_replacement(const SOL *offspr);
void preselection_replacement(SOL *p1, SOL *p2, const SOL *offspr);

void replacement(SOL *p1, SOL *p2, const SOL *offspr){
    switch (Params.replacement){
    case Random:
        random_replacement(offspr);
        break;
    case Worst:
        worst_replacement(offspr);
        break;
    case Preselection:
        preselection_replacement(p1, p2, offspr);
        break;
    }
	random_replacement(offspr);
}

// replace one solution from the population with the new offspring
// currently any random solution can be replaced
void random_replacement(const SOL *offspr){
	int i, p = rand() % Psize;
    memcpy(&Population[p], offspr, sizeof(SOL));
}

void worst_replacement(const SOL *offspr){
    int worst = Psize - 1; // sorted population
    memcpy(&Population[worst], offspr, sizeof(SOL));
}

void preselection_replacement(SOL *p1, SOL *p2, const SOL *offspr){
    if (p1->f > p2->f)
        memcpy(p1, offspr, sizeof(SOL));
    else
        memcpy(p2, offspr, sizeof(SOL));
}
