/**
 * Author: Paula Vidas
 * Date: 2024-11-09
 * Description: Computes DP of the form $dp[i] = a[i] + \sum_{j < i} dp[j] \cdot b[i-j]$.
 * Result is stored in $a$. Make sure that size is a power of 2.
 * Time: $O(n \log^2 n)$
 * Status: works, but speed could be improved...
 */
#pragma once

void OnlineFFT(vector<ModInt>& a, int a_lo, vector<ModInt>& b, int b_lo, int sz) {
    if (sz == 1) return;
    OnlineFFT(a, a_lo, b, b_lo, sz / 2);
    auto c = Mul({a.begin() + a_lo, a.begin() + a_lo + sz / 2}, {b.begin() + b_lo, b.begin() + b_lo + sz});
    for (int i = sz / 2; i < sz; i++)
        a[a_lo + i] += c[i];
    OnlineFFT(a, a_lo + sz / 2, b, b_lo, sz / 2);
}

