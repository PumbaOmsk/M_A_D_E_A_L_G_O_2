/**
 * Введенский Вячеслав В. MADE-ML-22
 *
A. Сумма простая
запрос «сумма чисел на отрезке». Массив не меняется. Запросов много. Отвечать на каждый запрос следует за O(1).
Входные данные: Размер массива — n и числа x, y, a0, порождающие массив a:
Далее следует количество запросов m и числа z, t, b0, порождающие массив b: .
Запросы: i-й из них — найти сумму на отрезке от min(c2i, c2i + 1) до max(c2i, c2i + 1) в массиве a.
Ограничения: 1 ≤ n ≤ 10^7, 0 ≤ m ≤ 10^7. Все числа целые от 0 до 2^16. t может быть равно  - 1.
Выведите сумму всех сумм.
 */
#include <iostream>
#include <vector>

using namespace std;

/**
 * Генерация вектора чисел А.
 * @param n длина вектора.
 * @param x порождающие числа.
 * @param y порождающие числа.
 * @param a0 начало вектора.
 * @return вектор сгенерированных чисел.
 */
vector<unsigned long> generateA(int n, long long x, long long y, unsigned long a0) {
    vector<unsigned long> a(n, 0);
    a[0] = a0;
    long mod = 1 << 16;
    for (long i = 1; i < n; ++i) {
        long long temp = x * a[i - 1] + y;
        a[i] = static_cast<unsigned long>(temp % mod);
    }
    return a;
}

/**
 * @param a вектор чисел.
 * @return вектор сумм подпоследовательностей.
 */
vector<unsigned long long> preparePrefixSum(const vector<unsigned long> &a) {
    auto n = a.size();
    vector<unsigned long long> sum(n, 0);
    sum[0] = a[0];
    for (int i = 1; i < n; ++i) {
        sum[i] = sum[i - 1] + a[i];
    }
    return sum;
}

/**
 * Запрос значения суммы элементов.
 * @param sum вектор сумм подпоследовательностей.
 * @param l начало отрезка.
 * @param r конец отрезка.
 * @return сумма элементов от l до r.
 */
unsigned long long rsq(const vector<unsigned long long> &sum, long l, long r) {
    if (l == 0)
        return sum[r];
    return sum[r] - sum[l - 1];
}

/**
 * Решение исходной задачи. Сумма сумм.
 * @param a исходный вектор чисел.
 * @param m количество запросов.
 * @param z порождающие для нахождения начала и конца отрезка.
 * @param t порождающие для нахождения начала и конца отрезка.
 * @param b0 порождающие для нахождения начала и конца отрезка.
 * @return сумма сумм.
 */
long long calcSumSumEx(const vector<unsigned long> &a, long m, long z, long t, long b0) {
    long n = static_cast<long>(a.size());
    auto sum = preparePrefixSum(a);
    long modB = 1 << 30;

    long b2i0 = b0;
    long b2i1 = (z * b2i0 + t) % modB;
    if (b2i1 < 0)
        b2i1 += modB;
    unsigned long long result = 0;
    for (long long i = 0; i < m; ++i) {
        long c2i = b2i0 % n;
        long c2i1 = b2i1 % n;
        long l;
        long r;
        if (c2i < c2i1) {
            l = c2i;
            r = c2i1;
        } else {
            l = c2i1;
            r = c2i;
        }
        unsigned long long s = rsq(sum, l, r);
        result += s;
        b2i0 = (z * b2i1 + t) % modB;
        if (b2i0 < 0)
            b2i0 += modB;

        b2i1 = (z * b2i0 + t) % modB;
        if (b2i1 < 0)
            b2i1 += modB;
    }
    return result;
}

int main() {
    long n, x, y, a0;
    long m, z, t, b0;
    cin >> n >> x >> y >> a0;
    cin >> m >> z >> t >> b0;
    auto a = generateA(n, x, y, static_cast<unsigned long>(a0));
    cout << calcSumSumEx(a, m, z, t, b0);
    return 0;
}
