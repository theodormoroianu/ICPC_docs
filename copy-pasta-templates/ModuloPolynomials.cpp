#include <bits/stdc++.h>
using namespace std;

template <class T>
T Pow(T x, auto y)
{
    if (y == 0)
        return T(1);
    if (y % 2)
        return Pow(x, y - 1) * x;
    T v = Pow(x, y / 2);
    return v * v;
}

const int MOD = 998244353;
const int kRoot = 5;

struct ModInt
{
    int x;

    constexpr ModInt(int64_t y = 0) : x(y % MOD)
    {
        if (x < 0)
            x += MOD;
    }
    ModInt &operator+=(ModInt that)
    {
        if ((x += that.x) >= MOD)
            x -= MOD;
        return *this;
    }
    ModInt &operator-=(ModInt that)
    {
        if ((x -= that.x) < 0)
            x += MOD;
        return *this;
    }
    ModInt &operator*=(ModInt that)
    {
        x = (int64_t(x) * that.x) % MOD;
        return *this;
    }
    ModInt operator+(ModInt that) const { return ModInt(*this) += that; }
    ModInt operator-(ModInt that) const { return ModInt(*this) -= that; }
    ModInt operator*(ModInt that) const { return ModInt(*this) *= that; }
    ModInt operator/(ModInt that) const { return *this * that.inv(); }

    ModInt operator-() const { return ModInt(MOD - x); }

    ModInt inv() const { return Pow(*this, MOD - 2); }
    static ModInt root(int order) { return Pow(ModInt(kRoot), (MOD - 1) / order); }

    explicit operator int() const { return x; }
    explicit operator bool() const { return x; }

    bool operator==(ModInt that) const { return x == that.x; }
    bool operator!=(ModInt that) const { return x != that.x; }
};

ModInt Factorial(size_t n)
{
    static vector<ModInt> data(1, 1);
    while (data.size() <= n)
    {
        data.push_back(data.back() * data.size());
    }

    return data[n];
}

// Computes the inverse, N needs to be relatively small.
ModInt InverseWithMemo(size_t n)
{
    static vector<ModInt> data(2, 1);
    while (data.size() <= n)
    {
        int x = data.size();
        const int kModulo = MOD;
        data.push_back(data[kModulo % x] * (kModulo - kModulo / x));
    }
    return data[n];
}

ModInt FactorialInverse(size_t n)
{
    static vector<ModInt> data(1, 1);
    while (data.size() <= n)
    {
        data.push_back(data.back() * InverseWithMemo(data.size()));
    }
    return data[n];
}

static void DFT(vector<ModInt> &line, bool invert = false)
{
    for (size_t i = 1, j = 0; i < line.size(); ++i)
    {
        int bit = line.size() / 2;
        for (; j & bit; bit /= 2)
            j ^= bit;
        j ^= bit;
        if (i < j)
            swap(line[i], line[j]);
    }

    for (size_t window = 1; window < line.size(); window *= 2)
    {
        ModInt root = ModInt::root(window * 2);
        if (invert)
            root = root.inv();

        for (size_t begin = 0; begin < line.size(); begin += 2 * window)
        {
            ModInt w = 1;
            for (size_t i = 0; i < window; ++i, w = w * root)
            {
                ModInt a = line[begin + i];
                ModInt b = line[begin + window + i] * w;
                line[begin + i] = a + b;
                line[begin + window + i] = a - b;
            }
        }
    }
}

struct Power
{
    size_t exp;
    Power operator^(size_t x) { return Power{exp * x}; }
} X{1};

struct Poly
{
    Poly() {}
    Poly(ModInt x) : A(1, x) {}
    Poly(vector<ModInt> A) : A(std::move(A)) { clean(); }
    Poly(Power p) : A(p.exp + 1, 0) { A[p.exp] = 1; }
    Poly(std::initializer_list<ModInt> list) : A(list) { clean(); }

    bool operator==(const Poly &that) const { 
        // Invoke the "clean" function.
        return Poly(A).A == Poly(that.A).A;
    }

    Poly &operator+=(const Poly &that)
    {
        A.resize(max(A.size(), that.A.size()));
        for (size_t i = 0; i < that.A.size(); ++i)
            A[i] += that.A[i];
        clean();
        return *this;
    }

    Poly &operator-=(const Poly &that)
    {
        A.resize(max(A.size(), that.A.size()));
        for (size_t i = 0; i < that.A.size(); ++i)
            A[i] -= that.A[i];
        clean();
        return *this;
    }

    Poly operator+(const Poly &that) const { return Poly(*this) += that; }
    Poly operator-(const Poly &that) const { return Poly(*this) -= that; }
    Poly operator-() const
    {
        auto B = A;
        for (auto &x : B)
            x = -x;
        return B;
    }

    Poly operator*(Poly that) const
    {
        if (A.empty() || that.A.empty())
        {
            return Poly();
        }

        vector<ModInt> &B = that.A;
        if (min(A.size(), B.size()) < 15)
        {
            vector<ModInt> C(A.size() + B.size() - 1, 0);
            for (size_t i = 0; i < A.size(); ++i)
                for (size_t j = 0; j < B.size(); ++j)
                    C[i + j] += A[i] * B[j];
            return C;
        }

        vector<ModInt> A = this->A;

        size_t size = 1;
        while (size < A.size() + B.size())
            size *= 2;
        A.resize(size);
        B.resize(size);

        DFT(A);
        DFT(B);
        for (size_t i = 0; i < size; ++i)
            A[i] *= B[i];

        DFT(A, true);
        for (size_t i = 0; i < size; ++i)
            A[i] *= InverseWithMemo(size);
        return A;
    }
    Poly &operator*=(const Poly &that) { return (*this = *this * that); }

    Poly &operator*=(Power p)
    {
        A.insert(A.begin(), p.exp, 0);
        return *this;
    }
    Poly operator*(Power p) const { return Poly(*this) *= p; }

    Poly &operator/=(Power p)
    {
        if (A.size() < p.exp)
        {
            A.clear();
        }
        else
        {
            A.erase(A.begin(), A.begin() + p.exp);
        }
        return *this;
    }
    Poly operator/(Power p) const { return Poly(*this) /= p; }

    Poly &operator%=(Power p)
    {
        A.resize(min(A.size(), p.exp));
        return *this;
    }
    Poly operator%(Power p) const { return Poly(*this) %= p; }

    Poly &operator*=(ModInt x)
    {
        for (auto &y : A)
            y *= x;
        return *this;
    }
    Poly operator*(ModInt x) const { return Poly(*this) *= x; }

    Poly &operator/=(ModInt x) { return *this *= x.inv(); }
    Poly operator/(ModInt x) const { return Poly(*this) /= x; }

    Poly Derivative(int rank = 1) const
    {
        if (degree() + 1 < rank)
        {
            return Poly();
        }

        vector<ModInt> result(degree() + 1 - rank);
        for (int i = rank; i <= degree(); ++i)
            result[i - rank] = Factorial(i) * FactorialInverse(i - rank) * A[i];
        return result;
    }

    Poly Integral() const
    {
        vector<ModInt> result(degree() + 2);
        for (int i = 0; i <= degree(); ++i)
            result[i + 1] = A[i] * InverseWithMemo(i + 1);

        return result;
    }

    Poly log(size_t n)
    {
        assert(int(A[0]) == 1);
        return (Derivative() % (X ^ n) * Inverse(n)).Integral() % (X ^ n);
    }

    Poly exp(size_t n)
    {
        if (n == 1)
        {
            assert(int((*this)[0]) == 0);
            return ModInt(1);
        }

        auto B = exp((n + 1) / 2);
        auto A = *this % (X ^ n);
        return (B * (Poly(1) + A - B.log(n))) % (X ^ n);
    }

    Poly Inverse(size_t n) const
    {
        if (n == 1)
            return (*this)[0].inv();
        auto B = Inverse((n + 1) / 2);
        auto A = *this % (X ^ n);
        return (B * (Poly(2) - A * B)) % (X ^ n);
    }

    Poly sqrt(size_t n) const
    {
        if (n == 1)
        {
            auto x = (*this)[0];
            assert(x == 1); // no code for sqrt of random value
            return x;
        }

        auto B = sqrt((n + 1) / 2);
        auto A = *this % (X ^ n);
        return (B + A * B.Inverse(n)) / 2;
    }

    template <class Recurse>
    static Poly newton(size_t n, ModInt init, Recurse recurse)
    {
        if (n == 1)
        {
            return init;
        }

        auto B = newton((n + 1) / 2, init, recurse);
        return recurse(n, std::move(B));
    }

    int degree() const { return A.size() - 1; }

    void clean()
    {
        while (!A.empty() && !A.back())
            A.pop_back();
    }

    ModInt operator[](size_t index) const
    {
        if (index < A.size())
            return A[index];
        return 0;
    }

    vector<ModInt> A;
};

Poly operator+(ModInt X, Poly Y) { return Y + X; }
Poly operator-(ModInt X, Poly Y) { return Poly(X) - Y; }
Poly operator*(ModInt X, Poly Y) { return Y * X; }

// Binomial coeficients, combinari
ModInt Combinations(int N, int K)
{
    return Factorial(N) * FactorialInverse(K) * FactorialInverse(N - K);
}

int main()
{
    // Sample usage.
    ModInt a = 10;
    assert(-a == MOD - 10);
    assert(Factorial(100) == Factorial(101) * InverseWithMemo(101));

    Poly pol = {1, 2, 3};
    assert(pol.Derivative() == Poly({2, 6}));
    assert(pol * 3 == Poly({3, 6, 9}));
    assert(pol.Inverse(3) * pol % (X ^ 3) == Poly({1}));

    assert(Combinations(10, 3) == 120);
    
    cout << "OK\n";
}


