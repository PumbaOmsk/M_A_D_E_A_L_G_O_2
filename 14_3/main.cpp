/**
 * Введенский Вячеслав В. MADE-ML-22
 *
 C. ДНК Роботов.  В выходной файл выведите два числа — максимальное количество совпадающих символов и
 значение оптимального сдвига — неотрицательное количество символов второй ДНК, которые необходимо перенести из
 конца строки в её начало для достижения наилучшего совмещения.
 */

#include <iostream>
#include <complex.h>
#include <vector>
#include <map>

# define _PI        3.14159265358979323846    /* pi */

using namespace std;
using polytype = complex<double>;
const int _prepare = 20;
polytype _trig[20];  // предпосчитанные точки для разных степеней 2

void prepareSinCos() {
    for (int i = 0, n = 1; i < _prepare; ++i, n <<= 1) {
        double fi = 2 * _PI / n;
        _trig[i] = {cos(fi), sin(fi)};
    }
}

pair<int, long long> get2n(int l) {
    int p = 0;
    long long v = 1;
    while (v < l) {
        v = v << 1;
        p++;
    }
    return make_pair(p, v);
}

struct Polynom {
    Polynom(const Polynom &other, int start) : A(other.sz / 2) {
        unsigned long n = other.sz;
        sz = n / 2;
        for (int i = start, j = 0; i < n; i += 2, ++j) {
            A[j] = other.A[i];
        }
    }

    explicit Polynom(int _n) : A(_n) {
        sz = _n;
    }

    vector<polytype> A;
    int sz;

    void _mult(const Polynom &other) {
        for (int i = 0; i < sz; ++i) {
            A[i] *= other.A[i];
        }
    }


    void _div(double d) {
        for (int i = 0; i < sz; ++i) {
            A[i] = A[i] / d;
        }
    }

    void _reverse() {
        for (int i = 1; i <= sz / 2; ++i) {
            swap(A[i], A[sz - i]);
        }
    }

    void fft() {
        if (sz == 1)
            return;
        Polynom Aeven(*this, 0);
        Polynom Aodd(*this, 1);

        Aeven.fft();
        Aodd.fft();

//    double fi = 2 * _PI / sz;
//    polytype w_cur(1, 0), w(cos(fi), sin(fi));
        polytype w_cur(1, 0);

        int nn = sz, i = -1;
        while (nn > 0) {
            nn >>= 1;
            i++;
        }
        polytype &w = _trig[i];


        for (int k = 0; k <= sz / 2 - 1; ++k) {
            polytype odd = Aodd.A[k] * w_cur;
            A[k] = Aeven.A[k] + odd;
            A[k + sz / 2] = Aeven.A[k] - odd;
            w_cur *= w;
        }
    }

    void ifft() {
        _reverse();
        fft();
        _div(sz);
    }
};

Polynom operator*(Polynom &p1, Polynom &p2) {
    p1.fft();
    p2.fft();
    p1._mult(p2);
    p1.ifft();
    return p1;

}

Polynom polyFromString(const string &s, char c, int n, bool reverse) {
    unsigned long l = s.length();
    Polynom a(n);
    for (int i = 0; i < l; ++i) {
        if (s[i] == c) {
            if (reverse)
                a.A[l - i - 1].real(1);
            else
                a.A[i].real(1);
        }
    }
    return a;
}

pair<int, int> solveFindSubstrProblem(const string &s1, const string &s2) {
    int n = static_cast<int>(get2n(s1.length()).second);
    int m = s2.length();

    auto pA1 = polyFromString(s1, 'A', n, false);
    auto pA2 = polyFromString(s2, 'A', n, true);
    auto c1 = pA1 * pA2;

    auto pC1 = polyFromString(s1, 'C', n, false);
    auto pC2 = polyFromString(s2, 'C', n, true);
    auto c2 = pC1 * pC2;

    auto pG1 = polyFromString(s1, 'G', n, false);
    auto pG2 = polyFromString(s2, 'G', n, true);
    auto c3 = pG1 * pG2;

    auto pT1 = polyFromString(s1, 'T', n, false);
    auto pT2 = polyFromString(s2, 'T', n, true);
    auto c4 = pT1 * pT2;

    int mx = 0, shift = 0;
    for (int k = 0; k < m; k++) {
        int i = m + k - 1;
        int c = int(c1.A[i].real() + 0.5) + int(c2.A[i].real() + 0.5) + int(c3.A[i].real() + 0.5) +
                int(c4.A[i].real() + 0.5);
        if (c > mx) {
            mx = c;
            shift = k;
        }
    }
    return make_pair(mx, shift);
}

int main() {
    prepareSinCos();
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    int M;
    cin >> M;
    string s, s2;
    cin >> s >> s2;

    string s1 = s + s;
    auto res = solveFindSubstrProblem(s1, s2);
    cout << res.first << " " << res.second << endl;

    return 0;
}
