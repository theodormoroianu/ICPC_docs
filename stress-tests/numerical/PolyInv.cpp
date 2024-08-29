#include <bits/stdc++.h>
using namespace std;

#include "../../content/numerical/PolyInv.h"
#include "../../content/numerical/PolyMul.h"

int main()
{
    mt19937 rnd(time(0));
    for (int t = 0; t < 100000; t++) {
        int l = rnd() % 100 + 1;
        int n = rnd() % 100 + 1;
        if (n < l)
            swap(n, l);

        vector <ModInt> v(l);
        for (auto& i : v)
            i.x = rnd() % MOD;

        auto inv = Inv(v, n);
        assert((int)inv.size() == n);

        auto mul = Mul(v, inv);
        mul.resize(n);

        assert(mul[0].x == 1);
        for (int i = 1; i < n; i++)
            assert(mul[i].x == 0);
        
        if (t % 1000 == 0)
            cout << t / 1000 << "%\n";
    }
    cout << "PASS\n";
}