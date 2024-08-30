#include <bits/stdc++.h>
using namespace std;

#include "../../content/data-structures/LineContainer.h"


int main()
{
    mt19937 rnd(time(0));

    const int NR_TESTS = 1'000'000;
    for (int test = 0; test <= NR_TESTS; test++) {
        if (test % (NR_TESTS / 100) == 0)
            cerr << test / (NR_TESTS / 100) << "%\n";

        int N = rnd() % 100 + 1;
        LineContainer container;
        vector <pair <int, int>> lines;

        for (int i = 0; i < N; i++) {
            int a = rnd() % 100 - 50;
            int b = rnd() % 100 - 50;
            container.InsertLine(a, b);
            lines.push_back({a, b});
        }

        // Queries.
        const int M = 1000;
        for (int i = 0; i < M; i++) {
            int x = rnd() % 1000 - 500;
            int best = -1e9;
            for (auto [a, b] : lines)
                best = max(best, a * x + b);
            
            assert(container.EvalMax(x) == best);
        }
        
    }
	cout<<"Tests passed!"<<endl;
}
