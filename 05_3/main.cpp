/**
 * Введенский Вячеслав В. MADE-ML-22
 *
C. RMQ2
Входные данные: В первой строке находится число n — размер массива. (1 ≤ n ≤ 10^5)
 Во второй строке находится n чисел ai — элементы массива. Далее содержится описание операций,
 их количество не превышает 2·105. В каждой строке находится одна из следующих операций:
    set i j x — установить все a[k], i ≤ k ≤ j в x.
    add i j x — увеличить все a[k], i ≤ k ≤ j на x.
    min i j — вывести значение минимального элемента в массиве на отрезке с i по j, гарантируется, что (1 ≤ i ≤ j ≤ n).
Все числа во входном файле и результаты выполнения всех операций не превышают по модулю 1018.
Выходные данные: Выведите последовательно результат выполнения всех операций min.
 */
#include <iostream>
#include <vector>
#include <random>

/**
 * Максимальное, неопределенное значение.
 */
const long long INF = 9 * static_cast<long long>(1e18);

using namespace std;

/**
 * Получить элемент из дерева.
 * @param T вектор дерева отрезков.
 * @param updSet вектор операций изменения (set).
 * @param updAdd вектор операций изменения (add).
 * @param v номер вершины.
 * @return элемент в вершине.
 */
long long get(const vector<long long> &T, const vector<long long> &updSet, const vector<long long> &updAdd, int v) {
    if (updSet[v] != INF) {
        return updSet[v] + updAdd[v];
    }
    return T[v] + updAdd[v];
}

/**
 * Протолкнуть изменения ниже.
 * @param l левая граница.
 * @param r правая граница.
 */
void push(vector<long long> &T, vector<long long> &updSet, vector<long long> &updAdd, int v, int l, int r) {
    if (l == r) {
        if (updSet[v] != INF) {
            T[v] = updSet[v];
        }
        T[v] += updAdd[v];
    } else {
        if (updSet[v] != INF) {
            updSet[2 * v + 1] = updSet[v];
            updSet[2 * v + 2] = updSet[v];

            updAdd[2 * v + 1] = updAdd[v];
            updAdd[2 * v + 2] = updAdd[v];
        } else {
            updAdd[2 * v + 1] += updAdd[v];
            updAdd[2 * v + 2] += updAdd[v];
        }
        T[v] = min(get(T, updSet, updAdd, 2 * v + 1), get(T, updSet, updAdd, 2 * v + 2));
    }
    updSet[v] = INF;
    updAdd[v] = 0;
}

/**
 * вернуть минимальный элемент на подотрезке.
 * @param a левая граница исходного отрезка.
 * @param b правая граница исходного отрезка.
 * @return минимальный элемент.
 */
long long
rmq(vector<long long> &T, vector<long long> &updSet, vector<long long> &updAdd, int v, int l, int r, int a, int b) {
    push(T, updSet, updAdd, v, l, r);
    if (l > b || r < a) {
        return INF;
    }
    if (l >= a && r <= b) {
        return get(T, updSet, updAdd, v);
    }
    int m = (l + r) >> 1;
    auto lv = rmq(T, updSet, updAdd, 2 * v + 1, l, m, a, b);
    auto rv = rmq(T, updSet, updAdd, 2 * v + 2, m + 1, r, a, b);
    return min(lv, rv);
}

/**
 * групповая операция обновления - установить значение  на отрезке.
 * @param x значение
 */
void updateSet(vector<long long> &T, vector<long long> &updSet, vector<long long> &updAdd, int v, int l, int r,
               int a, int b, long long x) {
    push(T, updSet, updAdd, v, l, r);
    if (l > b || r < a) {
        return;
    }
    if (l >= a && r <= b) {
        updSet[v] = x;
        updAdd[v] = 0;
        return;
    }
    int m = (l + r) >> 1;
    auto v1 = 2 * v + 1;
    updateSet(T, updSet, updAdd, v1, l, m, a, b, x);
    updateSet(T, updSet, updAdd, v1+1, m + 1, r, a, b, x);
    T[v] = min(get(T, updSet, updAdd, v1), get(T, updSet, updAdd, v1+1));
}

/**
 * групповая операция обновления - добавить значение на отрезке.
 * @param x значение
 */
void updateAdd(vector<long long> &T, vector<long long> &updSet, vector<long long> &updAdd, int v, int l, int r,
               int a, int b, long long x) {
    push(T, updSet, updAdd, v, l, r);
    if (l > b || r < a) {
        return;
    }
    if (l >= a && r <= b) {
        updAdd[v] += x;
        return;
    }
    int m = (l + r) >> 1;
    updateAdd(T, updSet, updAdd, 2 * v + 1, l, m, a, b, x);
    updateAdd(T, updSet, updAdd, 2 * v + 2, m + 1, r, a, b, x);
    T[v] = min(get(T, updSet, updAdd, 2 * v + 1), get(T, updSet, updAdd, 2 * v + 2));
}

int main() {
    int n;
    cin >> n;
    int bound = 1;
    while (bound < n) {
        bound = bound << 1;
    }
    long long size = 2 * bound - 1;
    vector<long long> T(size, INF);
    vector<long long> updSet(size, INF);
    vector<long long> updAdd(size, 0);
    for (int i = 0; i < n; ++i) {
        cin >> T[bound - 1 + i];
    }
    for (int v = bound - 2; v >= 0; v--) {
        T[v] = min(T[2 * v + 1], T[2 * v + 2]);
    }

    for (int row = 0; row < 3 * 1e5; ++row) {
        string command;
        long i, j;
        cin >> command >> i >> j;
        if (command == "min") {
            cout << rmq(T, updSet, updAdd, 0, 0, bound - 1, i - 1, j - 1) << endl;
        } else {
            long long x;
            cin >> x;
            if (command == "set") {
                updateSet(T, updSet, updAdd, 0, 0, bound - 1, i - 1, j - 1, x);
            } else { // "add"
                updateAdd(T, updSet, updAdd, 0, 0, bound - 1, i - 1, j - 1, x);
            }
        }
    }
    return 0;
}