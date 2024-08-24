#include "../utilities/template.h"

#include "../../content/data-structures/FenwickTree2D.h"

int main() {
	rep(it,0,1000000) {
		Fenwick2D ft(12);
		vector<tuple<int, int, int>> upd;
		int c = rand() % 20;
		rep(i,0,c) {
			upd.emplace_back(rand() % 12, rand() % 12, rand() % 10 - 5);
		}

		vector<vi> grid(12, vi(12)), sumto(13, vi(13));
		for(auto &pa: upd)
			ft.FakeUpdate(get<0>(pa), get<1>(pa));
		ft.Build();
		for(auto &pa: upd) {
			grid[get<0>(pa)][get<1>(pa)] += get<2>(pa);
			ft.Update(get<0>(pa), get<1>(pa), get<2>(pa));
		}

		rep(i,0,12) {
			rep(j,0,12) {
				ll v = ft.Query(i, j);
                sumto[i][j] = grid[i][j];
                if (i)
                    sumto[i][j] += sumto[i-1][j];
                if (j)
                    sumto[i][j] += sumto[i][j-1];
                if (i && j)
                    sumto[i][j] -= sumto[i-1][j-1];
                assert(v == sumto[i][j]);
			}
		}
	}
	cout<<"Tests passed!"<<endl;
}
