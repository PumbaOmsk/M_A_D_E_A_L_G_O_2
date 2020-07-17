/**
 * Введенский Вячеслав В. MADE-ML-22
 *
 B. Дуэль
 Выведите количество способов выбрать стартовую точку и места для стрельбы согласно правилам дуэли.
 */

#include <iostream>
#include <complex.h>
#include <vector>
#include <map>

# define _PI        3.14159265358979323846    /* pi */

using namespace std;
using polytype = complex<double>;

pair<int, long long> get2n(int l) {
    int p = 0;
    long long v = 1;
    while (v <= l) {
        v = v << 1;
        p++;
    }
    return make_pair(p, v);
}

struct Polynom {
    Polynom(const string &s, int n) {
        A.resize(n);
        int l = static_cast<int>(s.length());
        for (int i = 0; i < l; ++i) {
            A[i] = {(double) s[l - i - 1] - '0', 0};
        }
    }

    Polynom(const vector<polytype> &v, int start) {
        unsigned long n = v.size();
        A.resize(n / 2);
        for (int i = start, j = 0; i < n; i += 2, ++j) {
            A[j] = v[i];
        }
    }

    explicit Polynom(int _n) {
        A.resize(_n);
    }

    vector<polytype> A;

    int size() const {
        return static_cast<int>(A.size());
    }

    Polynom even() const {
        return Polynom(A, 0);
    }

    Polynom odd() const {
        return Polynom(A, 1);
    }

    Polynom mult(const Polynom &other) const {
        int n = size();
        Polynom res(n);
        for (int i = 0; i < n; ++i) {
            res.A[i] = A[i] * other.A[i];
        }
        return res;
    }

    void _div(double d) {
        int n = size();
        for (int i = 0; i < n; ++i) {
            A[i] = A[i] / d;
        }
    }

    void _reverse() {
        int n = size();
        for (int i = 1; i <= n / 2; ++i) {
            swap(A[i], A[n - i]);
        }
    }
};

void fft(Polynom &A) {
    int n = A.size();
    if (n == 1)
        return;
    auto Aeven = A.even();
    auto Aodd = A.odd();
    fft(Aeven);
    fft(Aodd);

    double fi = 2 * _PI / n;
    polytype w_cur(1, 0), w(cos(fi), sin(fi));
    for (int k = 0; k <= n / 2 - 1; ++k) {
        polytype odd = Aodd.A[k] * w_cur;
        A.A[k] = Aeven.A[k] + odd;
        A.A[k + n / 2] = Aeven.A[k] - odd;
        w_cur *= w;
    }

}

void ifft(Polynom &A) {
    A._reverse();
    fft(A);
    A._div(A.size());
}

Polynom operator*(Polynom &p1, Polynom &p2) {
    fft(p1);
    fft(p2);
    auto fc = p1.mult(p2);
    ifft(fc);
    return fc;
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    string s;
    cin >> s;
    int n = static_cast<int>(get2n(2 * s.length()).second);
    Polynom p1(s, n);
    Polynom p2(s, n);
    auto c = p1 * p2; // возведем полином с деревьями в квадрат
    long long res = 0;
    for (int i = 0; i < c.size(); ++i) {
        long r = int(c.A[i].real() + 0.5);
        if (r % 2 == 1) { // для каждого нечетного значения - будет 2*X+1, X - количество симметричных деревьев
            res += (r - 1) / 2;
        }
    }
    cout << res << endl;
    return 0;
}