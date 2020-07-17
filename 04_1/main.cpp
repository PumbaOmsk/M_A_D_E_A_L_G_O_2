/**
 * Введенский Вячеслав В. MADE-ML-22
 *
A. Первообразный корень по простому модулю
Дано простое нечётное число p. Требуется найти минимальный первообразный корень по модулю p.
Входные данные: Первая строка входного файла содержит простое число p(3≤p≤10^9).
Выходные данные: Выведите ответ на поставленную задачу.
 */
#include <iostream>
#include <list>
#include <vector>

/**
 * Максимальное значение входных данных.
 */
const int MAX_VALUE = int(1e9) + 1;

using namespace std;

/**
 * Проверка: является ли v простым числом.
 * @param primes список простых чисел.
 */
bool CheckPrimes(long v, const list<long> &primes) {
    for (long p:primes) {
        if (v % p == 0)
            return false;
    }
    return true;
}

/**
 * Ищет простые числа не больше, чем max.
 * @param max максимум.
 * @return список простых чисел не более max.
 */
list<long> CalcPrimes(long max) {
    list<long> primes;
    for (long i = 2; i * i <= max; ++i) {
        if (CheckPrimes(i, primes)) {
            primes.push_back(i);
        }

    }
    return primes;
}

/**
 * Возведение в степень по модулю.
 * @param base
 * @param d
 * @param mod
 * @return
 */
long long ModPow(long long base, long long d, long long mod) {
    if (mod == 1)
        return 0;
    long long m = 1;
    base = base % mod;
    while (d > 0) {
        if (d % 2 == 1)
            m = (m * base) % mod;
        d = d >> 1;
        base = (base * base) % mod;
    }
    return m;
}

/**
 * Разложение числа на множители.
 * @param v число.
 * @param p вектор для множителей
 * @param k вектор для степеней
 * @param min_dev вектор минимальных делителей для чисел.
 * @return количество множителей.
 */
int Factorization(long v, vector<long> &p, vector<long> &k, const list<long> &primes) {
    int idx = -1;
    for (long prime:primes) {
        if (v % prime == 0) {
            idx++;
            p[idx] = prime;
            k[idx] = 1;
            v = v / prime;
        }
        while (v % prime == 0) {
            k[idx]++;
            v = v / prime;
        }
        if (v == 1) {
            break;
        }
    }
    if (v > 1) {
        idx++;
        p[idx] = v;
        k[idx] = 1;
    }
    idx++;
    return idx;
}

/**
 * Проверка корня на первообразность.
 * @param g кандидат.
 * @param p модуль.
 * @param base множители (основание факторизации).
 * @param last количество множителей.
 * @return true, если корень первообразный.
 */
bool CheckRoot(long g, long p,const vector<long> &base, int last) {
    for (int i = 0; i < last; ++i) {
        long d = (p-1)/base[i];
        if(ModPow(g,d,p)==1)
            return false;
    }
    return true;
}

/**
 * Вернуть минимальный первообразный корень по модулю p
 * @param p модуль.
 * @return минимальный первообразный корень.
 */
long PrimitiveRootsMin(long p) {
    auto primes = CalcPrimes(MAX_VALUE);
    vector<long> base(10, 0);
    vector<long> k(10, 0);
    int last = Factorization(p-1,base,k,primes);

    for (int g = 2; g < p; ++g) {
        if (CheckRoot(g, p, base, last)) {
            return g;
        }
    }
    return -1;
}

int main() {
    long p;
    cin >> p;
    cout << PrimitiveRootsMin(p) << endl;
    return 0;
}