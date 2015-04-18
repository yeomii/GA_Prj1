#include "ga.h"

extern int N;
extern Parameter Params;

void gen_rand_order_solution(SOL *s);
void gen_rand_locus_solution(SOL *s);
void normalize_solution(SOL *s);

void gen_init_solution(SOL *s){
    // random generation
    if (Params.represent == OrderBase){
        gen_rand_order_solution(s);
        normalize_solution(s);
    }
    else
        gen_rand_locus_solution(s);

	eval(s);
}

// generate a random order-based solution at s
void gen_rand_order_solution(SOL *s){
	int i;
	for (i = 0; i < N; i++) {
		s->ch[i] = i;
	}
	for (i = 0; i < N; i++) {
		int r = i + rand() % (N - i);	// r is a random number in [i..N-i)
		int tmp = s->ch[i]; s->ch[i] = s->ch[r]; s->ch[r] = tmp; // swap
	}
}

void gen_rand_locus_solution(SOL *s){
    gen_rand_order_solution(s);
    int *tmp = new int[N];
    memcpy(tmp, s->ch, N * sizeof(int));
    for (int i = 0; i < N; i++){
        s->ch[tmp[i]] = tmp[(i+1)%N];
    }
    delete[] tmp;
}

void normalize_solution(SOL *s){
    int zero_index = 0;
    for (int i = 0; i < N; i++){
        if (s->ch[i] == 0){
            zero_index = i;
            break;
        }
    }
    if (zero_index == 0)
        return;
    int *temp = new int[N];
    memcpy(temp, s->ch, N * sizeof(int));
    memcpy(s->ch, temp + zero_index, (N - zero_index) * sizeof(int));
    memcpy(s->ch + (N-zero_index), temp, zero_index * sizeof(int));
    delete[] temp;
}