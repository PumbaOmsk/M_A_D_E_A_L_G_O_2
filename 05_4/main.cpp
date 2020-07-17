/**
 * Введенский Вячеслав В. MADE-ML-22
 *
 Дан массив из n чисел. Требуется написать программу, которая будет отвечать на запросы следующего вида:
 найти минимум на отрезке между u и v включительно.
 n, m (1⩽n⩽10^5, 1⩽m⩽10^7) и a1 (0⩽a1<16714589)
 */
#include <iostream>
#include <vector>

/**
 * Максимальный элемент массива.
 */
const long MOD = 16714589;
using namespace std;

/**
 * Заполнение вектора элементов по условию.
 * @param n  количество элементов.
 * @param a0 0й элемент.
 * @return вектор.
 */
vector<long> GenerateA(long n, long a0) {
    vector<long> a(n);
    a[0] = a0;
    for (int i = 1; i < n; ++i) {
        a[i] = (23 * a[i - 1] + 21563) % MOD;
    }
    return a;
}

/**
 * Считаем SparseTable
 * @param a вектор элементов.
 * @return разряженая таблица.
 */
vector<vector<long>> CalcTable(const vector<long> &a) {
    auto n = a.size();
    long k_max = 1;
    long temp = 2;
    while (temp < n) {
        temp = temp << 1;
        k_max++;
    }
    vector<vector<long>> table(n, vector<long>(k_max));
    for (int i = 0; i < n; ++i) {
        table[i][0] = a[i];
    }
    for (int k = 1; k < k_max; ++k) {
        long k2 = 1 << (k - 1);

        for (int l = 0; l < n; ++l) {
            long m2 = MOD;
            if (l + k2 < n)
                m2 = table[l + k2][k - 1];
            table[l][k] = min(table[l][k - 1], m2);
        }
    }
    return table;
}

/**
 * @param table разряженая таблица.
 * @param u начало отрезка.
 * @param v конец отрезка.
 * @return min на отрезке.
 */
long rmq(const vector<vector<long>> &table, long u, long v) {
    long l, r;
    if (u < v) {
        l = u;
        r = v;
    } else {
        l = v;
        r = u;
    }
    long k = 1;
    long k2 = 2;
    while (k2 < r - l + 1) {
        k++;
        k2 = k2 << 1;
    }
    k--;
    k2 = k2 >> 1;
    auto m1 = table[l][k];
    auto m2 = table[r - k2 + 1][k];
    return min(m1, m2);
}


int main() {
    long n, m, a1, u, v;
    cin >> n >> m >> a1 >> u >> v;
    auto a = GenerateA(n, a1);
    auto table = CalcTable(a);


    long u_prev = 0, v_prev = 0, r_prev = 0;
    for (int i = 1; i <= m; ++i) {
        r_prev = rmq(table, u - 1, v - 1);
        u_prev = u;
        v_prev = v;
        u = ((17 * u + 751 + r_prev + 2 * i) % n) + 1;
        v = ((13 * v + 593 + r_prev + 5 * i) % n) + 1;
    }
    cout << u_prev << " " << v_prev << " " << r_prev << endl;
    return 0;
}