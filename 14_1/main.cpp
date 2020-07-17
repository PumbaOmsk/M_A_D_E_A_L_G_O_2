/**
 * Введенский Вячеслав В. MADE-ML-22
 *
 */

#include <iostream>
#include <complex.h>
#include <vector>
#include <map>

# define _PI        3.14159265358979323846    /* pi */

using namespace std;
using polytype = complex<double>;
const int _prepare = 20;
polytype _trig[20];
void prepareSinCos(){
    for (int i = 0,n=1; i < _prepare; ++i,n<<=1) {
        double fi = 2*_PI/n;
        _trig[i]={cos(fi),sin(fi)};
    }
}

pair<int, long long> get2n(int l) {
    int p = 0;
    long long v = 1;
    while (v <= l) {
        v = v << 1;
        p++;
    }
    return make_pair(p, v);
}

struct LongInt {
    explicit LongInt(const string &s) {
        int shift = 0;
        if (s[0] == '-') {
            shift = 1;
            negative = true;
        }
        l = static_cast<int>(s.length() - shift);
        for (int i = 0; i < l; ++i) {
            A.emplace_back(s[l - i - 1 + shift] - '0');
        }
    }

    explicit LongInt(int _n) {
        A.resize(_n);
        l = _n;
    }

    vector<int> A;
    bool negative = false;
    int l; // длина исходного числа

    void print() {
        if (negative)
            cout << "-";
        for (int i = size() - 1; i >= 0; i--) {
            cout << A[i];
        }
        cout << endl;
    }

    int size() const {
        return static_cast<int>(A.size());
    }


};

struct Polynom {
    Polynom(const vector<polytype> &v, int start) {
        unsigned long n = v.size();
        A.resize(n / 2);
        for (int i = start, j = 0; i < n; i += 2, ++j) {
            A[j] = v[i];
        }
    }

    explicit Polynom(int _n) {
        A.resize(_n);
//        for (int i = 0; i < _n; ++i) {
//            A[i](0, 0);
//            A.emplace_back(0, 0);
//        }
    }

    Polynom(const LongInt &v, int _n) {
        int n = static_cast<int>(get2n(_n).second);
        A.resize(n);
        for (int i = 0; i < v.size(); ++i) {
            A[i] = {(double) v.A[i], 0};
        }
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

    LongInt asLongInt(bool neg) {
        int n = size();
        LongInt res(n);
        int shift = 0;
        for (int i = 0; i < n; ++i) {
            int r = int(A[i].real() + 0.5) + shift;
            shift = r / 10;
            r = r % 10;
            res.A[i] = r;
        }
        int last = n;
        while (last > 0 && res.A[--last] == 0);
        res.negative = neg;
        if (last == 0 && res.A[0] == 0) {
            res.negative = false;
        }
        res.A.resize(last + 1);

        return res;
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

//    double fi = 2 * _PI / n;
//    polytype w_cur(1, 0), w(cos(fi), sin(fi));
    polytype w_cur(1,0);
    int nn=n,i=-1;
    while(nn>0){
        nn>>=1;
        i++;
    }
    polytype &w = _trig[i];

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

LongInt operator*(const LongInt &a, const LongInt &b) {
    int n = a.size() + b.size();
    Polynom p1(a, n);
    Polynom p2(b, n);
    fft(p1);
    fft(p2);
    auto fc = p1.mult(p2);
    ifft(fc);
    return fc.asLongInt(a.negative != b.negative);
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    prepareSinCos();
    string s1, s2;
    cin >> s1 >> s2;
    int n = s1.length() + s2.length();
    LongInt p1(s1);
    LongInt p2(s2);
    auto c = p1 * p2;
    c.print();
    return 0;
}