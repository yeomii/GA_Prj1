#include "ga.h"

extern int N;
extern Parameter Params;

extern long long TimeLimit;
extern time_t BeginTime;

void uniform_mutation(SOL *s, double threshold);
void nonuniform_mutation(SOL *s);

void change_two(SOL *s, int idx);
void change_or(SOL *s, int idx);
void change_swap(SOL *s, int idx);

void mutation(SOL *s){
    if (Params.mutation_b > 1)
        nonuniform_mutation(s);
    else
        uniform_mutation(s, Params.mutation_t);

	eval(s);
}

void uniform_mutation(SOL *s, double threshold){
    for (int i = 0; i < N; i++){
        double r = ((double)rand() / (RAND_MAX));
        if (r < threshold){
            switch (Params.mutation){
            case ChangeMix:
            {
                int sel = rand() % 3;
                if (sel == 0)
                    change_two(s, i);
                else if (sel == 1)
                    change_or(s, i);
                else
                    change_swap(s, i);
                break;
            }
            case ChangeTwo:
                change_two(s, i);
                break;
            case ChangeOr:
                change_or(s, i);
                break;
            case ChangeSwap:
                change_swap(s, i);
                break;
            }
        }
    }
}

void nonuniform_mutation(SOL *s){
    int r = rand() % 2;
    double th = Params.mutation_t;
    if (Params.termination == LimitTime){
        long long time_lapse = time(NULL) - BeginTime;
        double time_portion = ((double)time_lapse / (double)TimeLimit);
        th = 1 - pow(th, pow(1 - time_portion, Params.mutation_b));
        th *= Params.mutation_t;
    }
    uniform_mutation(s, th);
}

void change_two(SOL *s, int idx){
    int target = rand() % N;
    if (idx == target)
        return;
    int left = idx < target ? idx : target;
    int right = idx > target ? idx : target;
    int gap = right - left;
    int *tmp = new int[gap];

    memcpy(tmp, s->ch + left, gap * sizeof(int));
    for (int i = 0; i < gap; i++)
        s->ch[left + i] = tmp[gap - i - 1];
    delete[] tmp;
}

void change_or(SOL *s, int idx){
    // move one point
    int target = rand() % N;
    if (target == idx)
        return;

    int *tmp = new int[N];
    memcpy(tmp, s->ch, N * sizeof(int));
    s->ch[target] = s->ch[idx];
    if (target < idx){
        memcpy(s->ch + (target + 1), tmp + target, (idx - target) *sizeof(N));
    }
    else{
        memcpy(s->ch + idx, tmp + (idx + 1), (target - idx) *sizeof(N));
    }

    delete[] tmp;
}

void change_swap(SOL *s, int idx){
    int target = rand() % N;
    int tmp = s->ch[idx];
    s->ch[idx] = s->ch[target];
    s->ch[target] = tmp;
}