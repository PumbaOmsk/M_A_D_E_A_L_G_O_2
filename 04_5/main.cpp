/**
 * Введенский Вячеслав В. MADE-ML-22
 *
 E. Факторизация
Дано натуральное число. Факторизуйте его, то есть представьте в виде произведения набора простых чисел.
Входные данные: В единственной строке записано единственное натуральное число N. 2 ≤ N ≤ 9·1018.
Выходные данные: Выведите в неубывающем порядке одно или несколько простых чисел, произведение которых равно N.
 */
#include <iostream>
#include <random>
#include <list>
#include <chrono>

using namespace std;
/**
 * Количество проверок на простоту.
 */
const int ROUND_COUNT = 10;

/**
 * умножение по модулю в обход переполнения разрядности.
 * @param a множитель.
 * @param b множитель.
 * @param p модуль.
 * @return
 */
unsigned long long mulmodu(unsigned long long a, unsigned long long b, unsigned long long MOD) {
    if (b == 0) return 0;
    unsigned long long res = mulmodu(a, b >> 1, MOD);
    res += res;
    res %= MOD;
    return (b & 1) ? (a + res) % MOD : res;
}

/**
 * вовзвдение в степень по модулю в обход переполнения разрядности.
 * @param a основание
 * @param b степень
 * @param p модуль.
 * @return
 */
unsigned long long powmodu(unsigned long long a, unsigned long long b, unsigned long long MOD) {
    if (b == 0) return 1;
    unsigned long long res = powmodu(a, b >> 1, MOD);
    res = mulmodu(res, res, MOD);
    return (b & 1) ? mulmodu(a, res, MOD) : res;
}

/**
 * Расширеный алгоритм Эвклида.
 * @param a число.
 * @param b число.
 * @param x координата x, удовлетворяющая уравнению.
 * @param y координата y, удовлетворяющая уравнению.
 * @return
 */
unsigned long long gcd(unsigned long long a, unsigned long long b, unsigned long long &x, unsigned long long &y) {
    if (a == 0) {
        x = 0;
        y = 1;
        return b;
    }
    unsigned long long x1, y1;
    unsigned long long d = gcd(b % a, a, x1, y1);
    x = y1 - (b / a) * x1;
    y = x1;
    return d;
}

/**
 * Вероятностная проверка большого числа на простоту.
 * @param m число.
 * @param k количество итераций проверки.
 * @return true, если простое.
 */
bool IsPrime(unsigned long long m, int k) {
    if (m == 2 || m == 3)
        return true;
    if (m == 1 || m % 2 == 0)
        return false;

    unsigned long long s = m - 1;
    long f = 0;
    while ((s % 2) == 0) {
        f++;
        s = s / 2;
    }
    uniform_int_distribution<unsigned long long> rnd(2, m - 2);
    default_random_engine re(17);

    for (int j = 0; j < k; ++j) {
        unsigned long long a = rnd(re);
        unsigned long long cur = powmodu(a, s, m);
        if (cur == 1 || cur == m - 1)
            continue;
        bool flag_continue = false;
        for (int i = 1; i < f; ++i) {
            cur = mulmodu(cur, cur, m);
            if (cur == 1)
                return false;
            if (cur == m - 1) {
                flag_continue = true;
                break;
            }
        }
        if (!flag_continue)
            return false;
    }
    return true;
}

/**
 * вычитание чисел по обычному модулю. из большего меньшее.
 * @param a число.
 * @param b число.
 * @return разность по модулю.
 */
unsigned long long SubAbs(unsigned long long a, unsigned long long b) {
    if (a < b)
        return b - a;
    return a - b;
}

/**
 * Поиск делителя методом Полларда.
 * @param v число.
 * @param max_iter количество итераций.
 * @return какой-то делитель.
 */
unsigned long long Pollard(unsigned long long v, int max_iter) {
    if (v % 2 == 0)
        return 2;
    if (v % 3 == 0)
        return 3;
    if (v % 5 == 0)
        return 5;
    uniform_int_distribution<unsigned long long> rnd(2, v - 2);
    default_random_engine re(static_cast<unsigned int>(chrono::steady_clock::now().time_since_epoch().count()));
    unsigned long long a0 = rnd(re);
    unsigned long long x, y;
    auto a1 = (mulmodu(a0, a0, v) + 1) % v;
    auto g = gcd(SubAbs(a1, a0), v, x, y);
    for (unsigned count = 0; count < max_iter && (g == 1 || g == v); ++count) {
        a0 = (mulmodu(a0, a0, v) + 1) % v;
        a1 = (mulmodu(a1, a1, v) + 1) % v;
        a1 = (mulmodu(a1, a1, v) + 1) % v;
        g = gcd(SubAbs(a1, a0), v, x, y);
    }
    return g;
}

/**
 * Разложение на простые множители больших чисел (рекурентная часть,может ошибаться).
 * @param v число.
 * @param devs список простых множителей.
 */
void Factorization(unsigned long long v, list<unsigned long long> &devs) {
    if (v == 1)
        return;
    if (IsPrime(v, ROUND_COUNT)) {
        devs.push_back(v);
        return;
    }
    auto dev = Pollard(v, 100000);
    if (dev == 1)
        return;
    Factorization(dev, devs);
    Factorization(v / dev, devs);
}

/**
 * Разложение на простые множители больших чисел (может ошибаться).
 * @param v число.
 * @return список простых множителей.
 */
list<unsigned long long> FactorizationBig(unsigned long long v) {
    list<unsigned long long> devs;
    Factorization(v, devs);
    devs.sort();
    return devs;
}

int main() {
    unsigned long long N;
    cin >> N;
    auto devs = FactorizationBig(N);
    for (unsigned long long dev:devs) {
        cout << dev << " ";
    }
    cout << endl;
    return 0;

}