/**
 * Введенский Вячеслав В. MADE-ML-22
 *
D. Проверка на простоту
Дано n натуральных чисел ai. Определите для каждого числа, является ли оно простым.
Входные данные: Программа получает на вход число n, 1 ≤ n ≤ 1000 и далее n чисел ai, 1 ≤ ai ≤ 1018.
Выходные данные: Если число ai простое, программа должна вывести YES, для составного числа программа должна вывести NO.
 */
#include <iostream>
#include <random>

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

int main() {
    int n;
    cin >> n;
    for (int i = 0; i < n; ++i) {
        unsigned long long a;
        cin >> a;
        cout << (IsPrime(a, ROUND_COUNT) ? "YES" : "NO") << endl;
    }
    return 0;
}
