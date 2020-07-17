//#define _DEBUG_
/**
 * Введенский Вячеслав В. MADE-ML-22
 *
B. Просеивай!
Для положительного целого n определим функции:
    d(n) — минимальный делитель n, больший 1, по определению положим d(1) = 0.
    s0(n) — количество различных делителей n.
    s1(n) — сумма всех делителей n.
    φ(n) — функция Эйлера, количество целых чисел k, таких что 1 ≤ k ≤ n и GCD(n, k) = 1.
Входные данные
В единственной строке записано число n (1 ≤ n ≤ 107).
Выходные данные
Выведите четыре числа: sum(1,n,d(k)),...
 */
#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <cmath>

using namespace std;
/**
 * Максимальное значение входных данных.
 */
//const int MAX_VALUE = 10000001;

/**
 * Разложение числа на множители.
 * @param v число.
 * @param p вектор для множителей
 * @param k вектор для степеней
 * @param min_dev вектор минимальных делителей для чисел.
 * @return количество множителей.
 */
int Factorization(long v, vector<long> &p, vector<long> &k, const vector<long> &min_dev) {
    int idx = -1;
    while (v != 1) {
        int prime = min_dev[v];
        if (idx == -1 || p[idx] != prime) {
            idx++;
            p[idx] = prime;
            k[idx] = 1;
            v = v / prime;
        } else {
            k[idx]++;
            v = v / prime;
        }
    }
    idx++;
    return idx;
}

/**
 * s0(n) — количество различных делителей n.
 * @param last количество делителей.
 * @param k степени.
 * @return количество разл. делителей.
 */
long long CalcS0(int last, const vector<long> &k) {
    long s0 = 1;
    for (int j = 0; j < last; ++j) {
        s0 *= (k[j] + 1);
    }
    return s0;
}

/**
 * Одновременно считаем:
 *  s1(n) — сумма всех делителей n.
 *  φ(n) — функция Эйлера, количество целых чисел k, таких что 1 ≤ k ≤ n и GCD(n, k) = 1.
 * @param last количество множителей.
 * @param p вектор множителей.
 * @param k вектор степеней.
 * @return пара (s1,значение функции Эйлера)
 */
pair<long long, long long> CalcS1Euler(int last, const vector<long> &p, const vector<long> &k) {
    long long s1 = 1;
    long long euler = 1;

    for (int i = 0; i < last; ++i) {
        long long sum = 1;
        long long degree = 1;
        for (int j = 1; j < k[i]; ++j) {
            degree *= p[i];
            sum += degree;
        }
        euler *= degree;
        euler *= (p[i] - 1);

        degree *= p[i];
        sum += degree;
        s1 *= sum;
    }
    return make_pair(s1, euler);
}

/**
 * Решение поставленной задачи.
 * @param n параметр.
 * @param min_dev вектор минимальных делителей для числа.
 * @return вектор из 4-х элементов.
 */
vector<long long> Solve(long n, const vector<long> &min_dev) {
    vector<long long> result(4, 0);
    result[0] = 0;
    result[1] = 1;
    result[2] = 1;
    result[3] = 1;
    vector<long> p(100, 0);
    vector<long> k(100, 0);
    for (long i = 2; i <= n; ++i) {
//    for (long i = n; i >= 2; --i) {
        std::fill(p.begin(), p.end(), 0);
        std::fill(k.begin(), k.end(), 0);
        auto last = Factorization(i, p, k, min_dev);
        result[0] += p[0];
        long long int s0 = CalcS0(last, k);
        result[1] += s0;

        auto s1_euler = CalcS1Euler(last, p, k);
        result[2] += s1_euler.first;
        result[3] += s1_euler.second;
    }
    return result;
}

/**
 * Разложение числа на множители.
 * @param v число.
 * @param p вектор для множителей
 * @param k вектор для степеней
 * @param min_dev вектор минимальных делителей для чисел.
 * @return количество множителей.
 */
//int FactorizationEx(long v, vector<long> &p, vector<long> &k,vector<long> &pp, const vector<int> &min_dev) {
int FactorizationEx(long v, vector<long> &p, vector<long> &k, const vector<long> &min_dev) {
    int idx = -1;
    while (v != 1) {
        long prime = min_dev[v];
        if (idx == -1 || p[idx] != prime) {
            idx++;
            p[idx] = prime;
            k[idx] = 0;
        }
        k[idx]++;
//        pp[idx] *= prime;
        v = v / prime;
    }
    idx++;
    return idx;
}

/**
 * Решение поставленной задачи.
 * @param n параметр.
 * @param min_dev вектор минимальных делителей для числа.
 * @return вектор из 4-х элементов.
 */
vector<long long> SolveEx(long n, const vector<long> &min_dev) {
    vector<long long> result(4, 0);
//    vector<long long> s0hist(static_cast<unsigned long>(n+1), -1);
//    vector<long long> s1hist(static_cast<unsigned long>(n+1), -1);
//    vector<long long> euhist(static_cast<unsigned long>(n+1), -1);
//    vector<vector<long>> powers(static_cast<unsigned long>(n + 1), vector<long>(25, -1));
    result[0] = 0;
    result[1] = 1;
    result[2] = 1;
    result[3] = 1;
    vector<long> p(10, 0);
//    vector<long> pp(100, 0);
    vector<long> k(10, 0);
    for (long i = 2; i <= n; ++i) {
//    for (long i = n; i >= 2; --i) {
//        std::fill(p.begin(), p.end(), 0);
//        std::fill(pp.begin(), pp.end(), 1);
//        std::fill(k.begin(), k.end(), 0);
        auto last = FactorizationEx(i, p, k, min_dev);
        result[0] += p[0];


//        if(last==1) {
        auto s1_euler = CalcS1Euler(last, p, k);
        long long int s0 = CalcS0(last, k);
        result[1] += s0;
        result[2] += s1_euler.first;
        result[3] += s1_euler.second;
//            s0hist[i] = s0;
//            s1hist[i] = s1_euler.first;
//            euhist[i] = s1_euler.second;
//        }
//        else {
//            long long ss0=1;
//            long long ss1=1;
//            long long ee1=1;
//
//            for (int j = 0; j < last; ++j) {
//                long ppp = pp[j];
////                if(powers[p[j]][k[j]]==-1) {
////                    for (int l = 1; l <= k[j]; ++l) {
////                        pp *= p[j];
////                    }
////                    powers[p[j]][k[j]] = pp;
////                } else {
////                    pp = powers[p[j]][k[j]];
////                }
//                ss0*=s0hist[ppp];
//                ss1*=s1hist[ppp];
//                ee1*=euhist[ppp];
//            }
//
//            s0hist[i] = ss0;
//            s1hist[i] = ss1;
//            euhist[i] = ee1;
//            result[1] += s0hist[i];
//            result[2] += s1hist[i];
//            result[3] += euhist[i];
//        }
    }
    return result;
}

/**
 * Решение поставленной задачи.
 * @param n параметр.
 * @param min_dev вектор минимальных делителей для числа.
 * @return вектор из 4-х элементов.
 */
vector<long long> SolveEx2(long n, const vector<long> &min_dev) {
    vector<long long> result(4, 0);
    result[0] = 0;
    result[1] = 1;
    result[2] = 1;
    result[3] = 1;
    vector<long> p(10, 0);
    vector<long> k(10, 0);
    vector<bool> f(static_cast<unsigned long>(n + 1), false);
    for (long i = 2; i <= n; ++i) {
        if (f[i])
            continue;

        auto last = FactorizationEx(i, p, k, min_dev);
        result[0] += p[0];
        auto s1_euler = CalcS1Euler(last, p, k);
        long long int s0 = CalcS0(last, k);
        result[1] += s0;
        result[2] += s1_euler.first;
        result[3] += s1_euler.second;
        f[i] = true;
        long old_i = i;

        for (int j = 0; j < last; ++j) {
            long old_k = k[j];
            long long curr = old_i * p[j];
            while (curr <= n) {
                k[j]++;
                if (!f[i]) {
                    result[0] += p[0];
                    auto s1_euler2 = CalcS1Euler(last, p, k);
                    long long int s02 = CalcS0(last, k);
                    result[1] += s02;
                    result[2] += s1_euler2.first;
                    result[3] += s1_euler2.second;
                    f[curr] = true;
                }
                curr *= p[j];
            }
            k[j] = old_k;
        }

    }
    return result;
}
/**
 * Решение поставленной задачи.
 * @param n параметр.
 * @param min_dev вектор минимальных делителей для числа.
 * @return вектор из 4-х элементов.
 */
vector<long long> SolveEx3(long n, const vector<long> &min_dev) {
    vector<long long> result(4, 0);
    vector<long long> hist1(n+1,0);
    vector<long long> hist2(n+1,0);
    vector<long long> hist3(n+1,0);
    result[0] = 0;
    result[1] = 1;
    result[2] = 1;
    result[3] = 1;
    vector<long> p(10, 0);
    vector<long> k(10, 0);
    for (long i = 2; i <= n; ++i) {
        result[0] += min_dev[i];
        if(min_dev[i]==i) {
            auto last = FactorizationEx(i, p, k, min_dev);
            long long int s0 = CalcS0(last, k);
            auto s1_euler = CalcS1Euler(last, p, k);
            result[1] += s0;
            result[2] += s1_euler.first;
            result[3] += s1_euler.second;
            hist1[i] =s0;
            hist2[i] =s1_euler.first;
            hist3[i] =s1_euler.second;
        } else {
            long temp = min_dev[i];
            long idx1 = 1;
            long j=i;
            while(min_dev[j]==temp){
                idx1 *= temp;
                j/=temp;
            }
            long idx2 = i/idx1;
            if(idx2==1){
                auto last = FactorizationEx(i, p, k, min_dev);
                long long int s0 = CalcS0(last, k);
                auto s1_euler = CalcS1Euler(last, p, k);
                result[1] += s0;
                result[2] += s1_euler.first;
                result[3] += s1_euler.second;
                hist1[i] =s0;
                hist2[i] =s1_euler.first;
                hist3[i] =s1_euler.second;
            }else {
                long long int s0 = hist1[idx1] * hist1[idx2];
                result[1] += s0;
                long long int s1 = hist2[idx1] * hist2[idx2];
                result[2] += s1;
                long long int eu = hist3[idx1] * hist3[idx2];
                result[3] += eu;
                hist1[i] =s0;
                hist2[i] =s1;
                hist3[i] =eu;
            }
        }
    }
    return result;
}
#ifdef _DEBUG_

#include <chrono>
#include <iostream>

class Timer {
    using clock_t = std::chrono::high_resolution_clock;
    using microseconds = std::chrono::microseconds;
public:
    Timer() : start_(clock_t::now()) {}

    ~Timer() = default;

    void print(const std::string &s, int count) {
        const auto finish = clock_t::now();
        const auto us =
                std::chrono::duration_cast<microseconds>
                        (finish - start_).count();
        std::cout << s << ": " << us / count << " us" << std::endl;
    }

    void print(const std::string &s) {
        print(s, 1);
    }

private:
    const clock_t::time_point start_;
};

#endif

/*
8000000

solve: 5426969 us
2080772003139 128395238 52637898294162 19453669439784
*/
int main() {
    long n;
    cin >> n;
    long max_value = n + 1;
    vector<long> min_dev(static_cast<unsigned long>(max_value), 0);
    list<long> primes;
    min_dev[1] = 0;
    // заполним вектор минимальных делителей для чисел.
    for (int i = 2; i < max_value; i++) {
        if (min_dev[i] == 0) {
            min_dev[i] = i;
            primes.push_back(i);
        }
        for (long p : primes) {
            if (p > min_dev[i] || i * p >= max_value) {
                break;
            }
            min_dev[i * p] = p;
        }
    }
#ifndef _DEBUG_
    auto res = SolveEx3(n, min_dev);
    for (long long r: res) {
        cout << r << " ";
    }
#endif

#ifdef _DEBUG_
    {
        Timer t;
        auto res = Solve(n, min_dev);
        t.print("solve");
        for (long long r: res) {
            cout << r << " ";
        }
    }
    {
        cout << endl;
        Timer t;
        auto res = SolveEx(n, min_dev);
        t.print("solveEx");
        for (long long r: res) {
            cout << r << " ";
        }
    }
    {
        cout << endl;
        Timer t;
        auto res = SolveEx2(n, min_dev);
        t.print("solveEx2");
        for (long long r: res) {
            cout << r << " ";
        }
    }
    {
        cout << endl;
        Timer t;
        auto res = SolveEx3(n, min_dev);
        t.print("solveEx3");
        for (long long r: res) {
            cout << r << " ";
        }
    }
#endif
    cout << flush;
    return 0;
}
