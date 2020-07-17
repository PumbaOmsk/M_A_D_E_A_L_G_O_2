/**
 * Введенский Вячеслав В. MADE-ML-22
 *
B. Дискретное логарифмирование
Даны натуральные числа a, b, и простое число n. Требуется найти дискретный логарифм b по основанию a по модулю n,
 то есть такое число x (0⩽x<n), что ax≡b(modn).
Входные данные: В первой строке заданы через пробел три целых числа a, b и n (0⩽a,b<n⩽10^12), Число nпростое.
Выходные данные: В первой строке выведите −1, если дискретного логарифма не существует.
 Иначе следует вывести его значение. Если ответ неоднозначен, разрешается выводить любой.
 */
#include <iostream>
#include <cmath>
#include <algorithm>
#include <map>

using namespace std;

/**
 * умножение по модулю в обход переполнения разрядности.
 * @param a множитель.
 * @param b множитель.
 * @param p модуль.
 * @return
 */
unsigned long long mulmodu(unsigned long long a, unsigned long long b, unsigned long long p) {
    if (b == 0) return 0;
    unsigned long long res = mulmodu(a, b >> 1, p);
    res += res;
    res %= p;
    return (b & 1) ? (a + res) % p : res;
}

/**
 * вовзвдение в степень по модулю в обход переполнения разрядности.
 * @param a основание
 * @param b степень
 * @param p модуль.
 * @return
 */
unsigned long long powmodu(unsigned long long a, unsigned long long b, unsigned long long p) {
    if (b == 0) return 1;
    unsigned long long res = powmodu(a, b >> 1, p);
    res = mulmodu(res, res, p);
    return (b & 1) ? mulmodu(a, res, p) : res;
}

/**
*  Дискретный логарифм (сначала baby, потом giant).
* @param a основание
* @param b число
* @param p модуль
*/
long long DiscreteLogBG(unsigned long long a, unsigned long long b, unsigned long long p) {
    unsigned long long n = 1 + (unsigned long long) (sqrt(p));

    map<unsigned long long, unsigned long long> m;
    unsigned long long bstep = 1;
    for (unsigned long long r = 0; r <= n; ++r) {
        m[bstep] = r;
        bstep = mulmodu(a, bstep, p);
    }

    unsigned long long gfactor = powmodu(a, (p - 2) * n, p);
    unsigned long long gstep = b;
    for (unsigned long long q = 0; q <= n; ++q) {
        if (m.count(gstep) > 0) {
            unsigned long long i = m[gstep];
            auto res = q * n + i;
            if (res < p) {
                return res;
            }
        }
        gstep = mulmodu(gstep, gfactor, p);
    }
    return -1;
}

int main() {
    long long a, b, n;
    cin >> a >> b >> n;
    cout << DiscreteLogBG(a, b, n) << endl;
    return 0;
}