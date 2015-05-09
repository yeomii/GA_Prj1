#include "ga.h"
#include <algorithm>

extern int N;
extern double Dist[MAXN][MAXN];

void two_opt(SOL *s) {
  bool improved = true;
  while (improved){
    improved = false;
    for (int i = 0; i < N; i++) {
      for (int j = i + 2; j < N; j++) {
        if (j - i >= N - 1)
          continue;
        int ith = s->ch[i], nexti = s->ch[i + 1];
        int jth = s->ch[j], nextj = s->ch[(j + 1) % N];
        double before = Dist[ith][nexti] + Dist[jth][nextj];
        double after = Dist[ith][jth] + Dist[nexti][nextj];
        if (after < before){
          s->f += (after - before);
          reverse(s->ch + (i + 1), s->ch + (j + 1));
          improved = true;
        }
      }
    }
  }
}