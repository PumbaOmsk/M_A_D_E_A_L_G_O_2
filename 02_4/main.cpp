/*
 * Введенский Вячеслав В. MADE-ML-22
 *
D. Продавец аквариумов (коммивояжер)
Продавец аквариумов для кошек хочет объехать n городов, посетив каждый из них ровно один раз.
Помогите ему найти кратчайший путь.

 Входные данные
Первая строка входного файла содержит натуральное число n (1 ≤ n ≤ 13) — количество городов.
 Следующие n строк содержат по n чисел — длины путей между городами.
В i-й строке j-е число — ai, j — это расстояние между городами i и j (0 ≤ ai, j ≤ 106; ai, j = aj, i; ai, i = 0).

 Выходные данные
В первой строке выходного файла выведите длину кратчайшего пути.
Во второй строке выведите n чисел — порядок, в котором нужно посетить города.
 */
#include <iostream>
#include <vector>
#include <stack>


using namespace std;
/**
 * Некоторое максимальное значение.
 */
const int MAX_VALUE = 1000000000;

/**
 * @param mask число
 * @param i номер бита.
 * @return значение бита в числе.
 */
int getBit(int mask, int i) {
    return (mask >> i) & 1;
}

/**
 * Поиск маршрута минимальной длины (незамкнутого!!!! )
 * @param n размерность.
 * @param distance матрица расстояний.
 * @return пара (длина минимального маршрута, стек с порядком вершин).
 */
pair<int, stack<int>> solve_route(size_t n, const vector<vector<int>> &distance) {
    auto size = 1 << n;
    vector<vector<int>> dp(n, vector<int>(size, MAX_VALUE));
    vector<vector<int>> p(n, vector<int>(size, -1));
    for (int i = 0; i < n; ++i) {
        dp[i][0] = 0;
    }
    for (int mask = 0; mask < size; ++mask) {
        for (int v = 0; v < n; ++v) {
            if (dp[v][mask] == MAX_VALUE)
                continue;
            for (int u = 0; u < n; ++u) {
                if (getBit(mask, u) == 0) {
                    auto idx = mask ^(1 << u);
                    auto new_dist = dp[v][mask] + distance[v][u];
                    if (new_dist < dp[u][idx]) {
                        dp[u][idx] = new_dist;
                        p[u][idx] = v;
                    }
                }
            }
        }
    }
    // находим в какой вершине закончился минимальный маршрут
    int mn = MAX_VALUE;
    int imn = -1;
    for (int i = 0; i < n; ++i) {
        if (dp[i][size - 1] < mn) {
            mn = dp[i][size - 1];
            imn = i;
        }
    }
    // восстанавливаем порядок городов
    stack<int> route;
    int prev = imn;
    int mask = size - 1;
    while (mask) {
        route.push(prev);
        int old_prev = prev;
        prev = p[old_prev][mask];
        mask = mask - (1 << old_prev);
    }

    return make_pair(dp[imn][size - 1], route);
}

int main() {
    size_t n;
    cin >> n;
    vector<vector<int>> distance(n, vector<int>(n, 0));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> distance[i][j];
        }
    }

    auto res = solve_route(n, distance);
    auto route = res.second;
    cout << res.first << endl;
    while (!route.empty()) {
        cout << route.top() + 1 << " ";
        route.pop();
    }
    return 0;
}
