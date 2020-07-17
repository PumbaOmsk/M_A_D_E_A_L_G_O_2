/**
 * Введенский Вячеслав В. MADE-ML-22
 *
E. Китайская теорема
Решите в целых числах систему уравнений
{x≡a(mod n) x≡b(mod m), где n и m взаимно просты.
 Среди решений следует выбрать наименьшее неотрицательное число.
Входные данные
Первая строка входных данных содержит число N, 1≤N≤10000, — количество тестов, для которых нужно решить задачу.
Следующие N строк содержат по четыре целых числа ai, bi, ni и mi (1≤ni,mi≤109, 0≤ai<ni, 0≤bi<mi).
Выходные данные
Для каждого из тестов выведите искомое наименьшее неотрицательное число xi
 */

#include <iostream>

using namespace std;

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
 * Нахождение целочисленной точки на прямой Ax+By+C=0.
 * @param a коэффициент.
 * @param b коэффициент.
 * @param c коэффициент.
 * @return пара(флаг успешности, пара(x,y))
 */
pair<bool, pair<long long, long long>> FindPointOnLine(long long a, long long b, long long c) {
    if (c == 0) {
        return make_pair(true, make_pair(0, 0));
    }
    long long x, y;
    auto g = gcd(a, b, x, y);
    if (c % g != 0) {
        return make_pair(false, make_pair(0, 0));
    }
    auto m = -c / g;
    return make_pair(true, make_pair(m * x, m * y));
}

/**
 * Решение простого варианта Китайской теоремы x=a mod n, x=b mod m.
 * @param a
 * @param b
 * @param n
 * @param m
 * @return x
 */
long long Solve(long long a, long long b, long long n, long long m) {
    auto res1 = FindPointOnLine(m, -n, -1);
    long long q1 = res1.second.first;
    auto res2 = FindPointOnLine(n, -m, -1);
    long long q2 = res2.second.first;
    long long mn = m * n;
    long long x = (m * ((a * q1) % n) + n * ((b * q2) % m)) % (mn);
    if (x < 0) {
        x += mn;
    }
    return x;
}

int main() {
    int N;
    cin >> N;
    for (int i = 0; i < N; ++i) {
        long long a, b, n, m;
        cin >> a >> b >> n >> m;
        cout << Solve(a, b, n, m) << endl;
    }
    return 0;
}

