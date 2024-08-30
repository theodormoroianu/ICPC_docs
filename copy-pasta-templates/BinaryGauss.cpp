#include <bits/stdc++.h>
using namespace std;

/**
 * Gauss elimination for xor.
 * The last element of the coeficients is the value of the equation.
 * I.e. 1 0 1 0 1 means x0 xor x2 = 1.
 *      1 1 1 1 0 means x0 xor x1 xor x2 xor x3 = 0.
 */
struct Gauss
{
    // Last item of coef is the value.
    vector<vector<int>> coefs;
    vector<int> pivot;
    int N;

    void cancelEq(int from, int to)
    {
        auto &f = coefs[from];
        auto &t = coefs[to];
        if (!t[pivot[from]])
            return;

        for (int i = 0; i <= N; i++)
            t[i] ^= f[i];
    }

    bool AddEq(vector<int> eq)
    {
        assert((int)eq.size() == N + 1);
        assert(coefs.size() == pivot.size());

        int eq_nr = coefs.size();
        coefs.push_back(eq);

        for (int i = 0; i < eq_nr; i++)
            cancelEq(i, eq_nr);

        int piv = 1e9;
        for (int i = 0; i < N; i++)
            if (coefs.back()[i])
                piv = min(piv, i);

        // Not possible.
        if (piv > N && coefs.back().back())
        {
            coefs.pop_back();
            return false;
        }

        // Eq can be ignored.
        if (piv > N)
        {
            coefs.pop_back();
            return true;
        }

        // Set pivot, cancel up.
        pivot.push_back(piv);
        for (int i = 0; i < eq_nr; i++)
            cancelEq(eq_nr, i);

        return true;
    }

    vector<int> GetSolution()
    {
        vector<int> ans(N);
        for (int i = 0; i < (int)coefs.size(); i++)
            ans[pivot[i]] = coefs[i].back();
        return ans;
    }

    Gauss(int N) : N(N) {}

    /**
     * For each element, returns 1 if the element is taken, 0 otherwise.
     */
    static vector<int> GetSubsetWithXor(vector<int> items, int target_xor)
    {
        Gauss gauss(items.size());
        for (int b = 1; b <= 1e9; b *= 2)
        {
            vector<int> eq;
            for (auto i : items)
            {
                if (i & b)
                    eq.push_back(1);
                else
                    eq.push_back(0);
            }
            if (target_xor & b)
                eq.push_back(1);
            else
                eq.push_back(0);

            if (!gauss.AddEq(eq))
                return {};
        }
        return gauss.GetSolution();
    }
};

int main()
{
    Gauss g(3);
    g.AddEq({1, 0, 1, 0});
    g.AddEq({1, 1, 1, 1});
    g.AddEq({1, 1, 0, 1});

    auto sol = g.GetSolution();
    assert(sol == vector<int>({0, 1, 0}));

    assert(Gauss::GetSubsetWithXor({ 1, 3, 8, 5 }, 12) == vector<int>({ 1, 0, 1, 1 }));

    cout << "OK\n";
}