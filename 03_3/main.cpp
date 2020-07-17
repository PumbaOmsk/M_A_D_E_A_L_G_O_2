/**
 * Введенский Вячеслав В. MADE-ML-22
 *
C. Взлом RSA
Входные данные
Программа получает на вход три натуральных числа: n, e, C, n ≤ 109, e ≤ 109, C < n.
 Числа n и e являются частью какой-то реальной схемы RSA, т.е. n является произведением двух простых
 и e взаимно просто с φ(n). Число C является результатом шифрования некоторого сообщения M.
Выходные данные
Выведите одно число M (0 ≤ M < n), которое было зашифровано такой криптосхемой.
 */

#include <iostream>
#include <vector>
#include <list>
#include <cmath>

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
    for (long i = 2; i < max; ++i) {
        if (CheckPrimes(i, primes)) {
            primes.push_back(i);
        }

    }
    return primes;
}

/**
 * Расширеный алгоритм Эвклида.
 * @param a число.
 * @param b число.
 * @param x координата x, удовлетворяющая уравнению.
 * @param y координата y, удовлетворяющая уравнению.
 * @return
 */
long long gcd(long long a, long long b, long long &x, long long &y) {
    if (a == 0) {
        x = 0;
        y = 1;
        return b;
    }
    long long x1, y1;
    long long d = gcd(b % a, a, x1, y1);
    x = y1 - (b / a) * x1;
    y = x1;
    return d;
}

/**
 * Разложение n на простые множители.
 * @param n
 * @param primes список простых чисел.
 * @return пара множителей
 */
pair<long long, long long> FindMultipliers(long long n, const list<long> &primes) {
    for (long p:primes) {
        if (n % p == 0)
            return make_pair(p, n / p);
    }
    return make_pair(n, 1);
}

long long ModPow(long long c, long long d, long long n) {
    if (n == 1)
        return 0;
    long long m = 1;
    c = c % n;
    while (d > 0) {
        if (d % 2 == 1)
            m = (m * c) % n;
        d = d >> 1;
        c = (c * c) % n;
    }
    return m;
}

/**
 * Декодирование сообщения RSA.
 * @param n часть открытого ключа.
 * @param e часть открытого ключа.
 * @param C зашифрованное сообщение.
 * @param primes список простых чисел.
 * @return дешифрованное сообщение.
 */
long long DecodeMessage(long n, long e, long C, const list<long> &primes) {
    auto factors = FindMultipliers(n, primes);
    long long p = factors.first;
    long long q = factors.second;

    long long fi = (p - 1) * (q - 1);
    long long d, t;
    gcd(e, fi, d, t);
    while (d < 0) {
        d += fi;
    }
    long long M = ModPow(C, d, n);
    return M;
}

int main() {
    long n, e, C;
    cin >> n >> e >> C;
    auto primes = CalcPrimes(static_cast<long>(sqrt(n)) + 1);
    cout << DecodeMessage(n, e, C, primes) << flush;
    return 0;
}
