/**
 * Author: Simon Lindholm
 * Date: 2019-12-28
 * License: CC0
 * Source: https://github.com/hoke-t/tamu-kactl/blob/master/content/data-structures/MoQueries.h
 * Description: Answer interval or tree path queries by finding an approximate TSP through the queries,
 * and moving from one query to the next by adding/removing points at the ends.
 * If values are on tree edges, change \texttt{step} to add/remove the edge $(a, c)$ and remove the initial \texttt{add} call (but keep \texttt{in}).
 * Time: O(N \sqrt Q)
 * Status: stress-tested
 */
#pragma once

void add(int ind, int end) { ... } // add a[ind] (end = 0 or 1)
void del(int ind, int end) { ... } // remove a[ind]
int calc() { ... } // compute current answer

vector<int> mo(vector<pair<int, int>> Q) {
	int L = 0, R = 0, blk = 350; // ~N/sqrt(Q)
	vector<int> s(Q.size()), res = s;
#define K(x) pair<int, int>(x.first/blk, x.second ^ -(x.first/blk & 1))
	iota(s.begin(), s.end(), 0);
	sort(s.begin(), s.end(), [&](int s, int t){ return K(Q[s]) < K(Q[t]); });
	for (int qi : s) {
		auto q = Q[qi];
		while (L > q.first) add(--L, 0);
		while (R < q.second) add(R++, 1);
		while (L < q.first) del(L++, 0);
		while (R > q.second) del(--R, 1);
		res[qi] = calc();
	}
	return res;
}

vector<int> moTree(vector<array<int, 2>> Q, vector<vector<int>>& ed, int root=0){
	int N = sz(ed), pos[2] = {}, blk = 350; // ~N/sqrt(Q)
	vector<int> s(Q.size()), res = s, I(N), L(N), R(N), in(N), par(N);
	add(0, 0), in[0] = 1;
	auto dfs = [&](int x, int p, int dep, auto& f) -> void {
		par[x] = p;
		L[x] = N;
		if (dep) I[x] = N++;
		for (int y : ed[x]) if (y != p) f(y, x, !dep, f);
		if (!dep) I[x] = N++;
		R[x] = N;
	};
	dfs(root, -1, 0, dfs);
#define K(x) pair<int, int>(I[x[0]] / blk, I[x[1]] ^ -(I[x[0]] / blk & 1))
	iota(s.begin(), s.end(), 0);
	sort(s.begin(), s.end(), [&](int s, int t){ return K(Q[s]) < K(Q[t]); });
	for (int qi : s) for (int end = 0; end < 2; end++) {
		int &a = pos[end], b = Q[qi][end], i = 0;
#define step(c) { if (in[c]) { del(a, end); in[a] = 0; } \
                  else { add(c, end); in[c] = 1; } a = c; }
		while (!(L[b] <= L[a] && R[a] <= R[b]))
			I[i++] = b, b = par[b];
		while (a != b) step(par[a]);
		while (i--) step(I[i]);
		if (end) res[qi] = calc();
	}
	return res;
}