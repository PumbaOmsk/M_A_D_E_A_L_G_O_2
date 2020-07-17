/**
 * Введенский Вячеслав В. MADE-ML-22
 *
B. RSQ
Входные данные: В первой строке находится число n — размер массива. (1 ≤ n ≤ 500 000) Во второй строке находится
 n чисел ai — элементы массива. Далее содержится описание операций, их количество не превышает 1 000 000.
 В каждой строке находится одна из следующих операций:
    set i x — установить a[i] в x.
    sum i j — вывести значение суммы элементов в массиве на отрезке с i по j, гарантируется, что (1 ≤ i ≤ j ≤ n).
Все числа во входном файле и результаты выполнения всех операций не превышают по модулю 10^18.
Выходные данные: Выведите последовательно результат выполнения всех операций sum.
 */
#include <iostream>
#include <vector>

using namespace std;

/**
 * @param i параметр.
 * @return i & (i+1)
 */
long funcF(long i) {
    auto res = i & (i + 1);
    if (res > i)
        return 0;
    return res;
}

/**
 * строит Дерево Фенвика.
 * @param a вектор элементов.
 * @return вектор дерева.
 */
vector<long long> fenvik(const vector<long long> &a) {
    auto n = a.size();
    vector<long long> T(n, 0);
    for (long i = 0; i < n; ++i) {
        long long sum = 0;
        for (long j = funcF(i); j <= i; ++j) {
            sum += a[j];
        }
        T[i] = sum;
    }
    return T;
}

/**
 * @param T вектор дерева.
 * @param i индекс.
 * @return элемент по индексу.
 */
long long get(const vector<long long> &T, long i) {
    long long res = 0;
    while (i >= 0) {
        res += T[i];
        i = funcF(i) - 1;
    }
    return res;
}

/**
 * Посчитать сумму на отрезке.
 * @param T вектор дерева.
 * @param l начало.
 * @param r конец.
 * @return сумма элементов на отрезке.
 */
long long rsq(const vector<long long> &T, long l, long r) {
    if (l == 0) {
        return get(T, r);
    }
    auto rv = get(T, r);
    auto lv = get(T, l - 1);
    return rv - lv;
}

/**
 * Добавить к i-му элементу dx.
 * @param T вектор дерева.
 * @param i индекс.
 * @param dx дельта.
 */
void add(vector<long long> &T, long i, long dx) {
    // надо еще в a прибавить... ?
    auto n = T.size();
    auto j = i;
    while (j < n) {
        T[j] += dx;
        j = j | (j + 1);
    }
}

/**
 * установить значение в элемент.
 * @param a вектор значений.
 * @param T вектор дерева.
 * @param i индекс.
 * @param x значение.
 */
void set(vector<long long> &a, vector<long long> &T, long i, long x) {
    auto d = x - a[i];
    a[i] = x;
    add(T, i, d);
}

int main() {
    int n;
    cin >> n;
    vector<long long> a(n, 0);
    for (int i = 0; i < n; ++i) {
        cin >> a[i];
    }

    auto T = fenvik(a);
    for (int row = 0; row < 1e6; ++row) {
        string command;
        long i, j;
        cin >> command >> i >> j;
        if (command == "sum") {
            cout << rsq(T, i - 1, j - 1) << endl;
        } else {
            set(a, T, i - 1, j);
        }
    }
    return 0;
}