#include "ga.h"
#include <set>
#include <vector>
using namespace std;

extern int N;
extern Parameter Params;

// combine the given parents p1 and p2
// and store the generated solution at c
// currently the child will be same as p1
void default_crossover(const SOL *p1, const SOL *p2, SOL *c);
void cycle_crossover(const SOL *p1, const SOL *p2, SOL *c);
void order_crossover(const SOL *p1, const SOL* p2, SOL *c);
void pmx_crossover(const SOL *p1, const SOL* p2, SOL *c);
void edge_recombination_crossover(const SOL *p1, const SOL* p2, SOL *c);

void crossover(const SOL *p1, const SOL *p2, SOL *c) {
    switch (Params.crossover){
    case Cycle:
        cycle_crossover(p1, p2, c);
        break;
    case Order:
        order_crossover(p1, p2, c);
        break;
    case PMX:
        pmx_crossover(p1, p2, c);
        break;
    case EdgeRecombination:
        edge_recombination_crossover(p1, p2, c);
        break;
    default:
        default_crossover(p1, p2, c);
    }
	eval(c);
}

void default_crossover(const SOL *p1, const SOL *p2, SOL *c){
	int i;
	for (i = 0; i < N; i++) {
		c->ch[i] = p1->ch[i];
	}
}

void cycle_crossover(const SOL *p1, const SOL *p2, SOL *c) {
	memset(c->ch, -1, N * sizeof(int));

	int *pos1 = new int[N], *pos2 = new int[N];
	for (int i = 0; i < N; i++) {
		pos1[p1->ch[i]] = i;
		pos2[p2->ch[i]] = i;
	}

	for (int i = 0; i < N; i++) {
		if (c->ch[i] != -1)
			continue;
		swap(p1, p2);
		swap(pos1, pos2);
		int j = i;
		while (1) {
			c->ch[j] = p2->ch[j];
			j = pos2[p1->ch[j]];
			if (i == j) break;
		}
	}

	delete[] pos1;
	delete[] pos2;

	// TODO: locus representation의 경우 하나의 cycle이 되도록 repair 해 주어야 한다.
}

void order_crossover(const SOL *p1, const SOL* p2, SOL *c) {
    int left = rand() % (N - 1), gap = rand() % (N - left - 1);
    int right = left + (gap != 0 ? gap : 1);

	int *has = new int[N];
    for (int i = 0; i < N; i++) has[i] = 0;
	for (int i = left; i < right; i++) {
		c->ch[i] = p1->ch[i];
		has[p1->ch[i]] = true;
	}
	int k = 0;
	for (int i = right; i != left; k = (k+1) % N) {
		if (has[p2->ch[k]]) continue;
		c->ch[i] = p2->ch[k];
        i = (i + 1) % N;
	}

	delete[] has;
}

void pmx_crossover(const SOL *p1, const SOL* p2, SOL *c) {
	int left = rand() % (N - 1), gap = rand() % (N - left - 1);
	int right = left + (gap != 0 ? gap : 1);

	int *pos = new int[N];
	memset(pos, -1, N * sizeof(int));
	for (int i = left; i < right; i++) {
		c->ch[i] = p1->ch[i];
		pos[p1->ch[i]] = i;
	}
	for (int i = right; i != left; i = (i+1) % N) {
		int val = p2->ch[i];
		while (pos[val] != -1) {
			int next_pos = pos[val];
			val = p2->ch[next_pos];
		}
		c->ch[i] = val;
	}

	delete[] pos;
}

namespace edge_recomb 
{
		// 도시 c1, c2를 인접 도시로 등록한다.
	void reg(vector< set<int> > &adj, int c1, int c2) {
		adj[c1].insert(c2);
		adj[c2].insert(c1);
	}

	// 도시 c1, c2를 인접 도시에서 삭제한다.
	void del(vector< set<int> > &adj, int c1, int c2) {
		adj[c1].erase(c2);
		adj[c2].erase(c1);
	}

	// 현재 도시에서 가야 할 다음 도시를 반환한다.
	int next(vector< set<int> > &adj, set<int> available_city, int city) {
		available_city.erase(city);
		if (adj[city].empty()) {
			return available_city.empty() ? -1 : *available_city.begin();
		}
		else {
			// 가장 인접 도시 수(즉 degree)가 적은 인접 도시를 고른다.
			int best_city, min_degree = 5;
            for (set<int>::iterator it = adj[city].begin(); it != adj[city].end(); it++) {
                int adj_city = *it;
				int degree = adj[adj_city].size();
				if (degree < min_degree) {
					min_degree = degree;
					best_city = adj_city;
				}
			}
			while (!adj[city].empty())
				del(adj, city, *adj[city].begin());
			return best_city;
		}
	}
}

// edge recombination은 order base representation에서만 의미를 가진다.
void edge_recombination_crossover(const SOL *p1, const SOL* p2, SOL *c) {
	vector< set<int> > adj(MAXN, set<int>());
	set<int> available_city;
	
	for (int i = 0; i < N; i++) {
		available_city.insert(p1->ch[i]);
		edge_recomb::reg(adj, p1->ch[i], p1->ch[(i + 1) % N]);
		edge_recomb::reg(adj, p2->ch[i], p2->ch[(i + 1) % N]);
	}

	for (int curr_city = 0, i = 0; curr_city != -1;) {
		c->ch[i++] = curr_city;
		curr_city = edge_recomb::next(adj, available_city, curr_city);
	}
}
