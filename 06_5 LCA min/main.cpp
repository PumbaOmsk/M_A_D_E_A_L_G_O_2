/**
 * Введенский Вячеслав В. MADE-ML-22
 *
E. Самое дешевое ребро (minonpath.in, minonpath.out)
Дано подвешенное дерево с корнем в первой вершине. Все ребра имеют веса (стоимости). Вам нужно ответить на M запросов
 вида "найти у двух вершин минимум среди стоимостей ребер пути между ними".
Входные данные: В первой строке задано целое число n — число вершин в дереве (1 ≤ n ≤ 2·10^5). В следующих n - 1 строках
записаны два целых числа x и y. Число x на строке i означает, что x — предок вершины i, y задает стоимость ребра (x < i; |y| ≤ 10^6).
Далее заданы m (0 ≤ m ≤ 5·105) запросов вида (x, y) — найти минимум на пути из x в y (1 ≤ x, y ≤ n; x ≠ y).
Выходные данные: Выведите ответы на запросы.
 */
#include <iostream>
#include <fstream>

using namespace std;
/// Неопределенный элемент массива.
const int INF = static_cast<const int>(1e7);

/**
 * Предподсчёт массива jmp - куда прыгаем.
 * @param n количество вершин в графе.
 * @param p массив предков.
 * @param cost массив стоимостей перехода к родителю.
 * @param deep глубина массива прыжков.
 * @param jmp двумерный массив [n][deep]. [v][k] куда прыгнем из вершины v длиной 2^k.
 * @param min_cost минимальные стоимости при соответствующем прыжке.
 */
void Precalc(int n, const int *p, const int *cost, int deep, int **jmp, int **min_cost) {
    for (int v = 0; v < n; ++v) {
        jmp[v][0] = p[v];
        min_cost[v][0] = cost[v];
    }
    for (int k = 1; k < deep; ++k) {
        for (int v = 0; v < n; ++v) {
            auto parent = jmp[v][k - 1];
            if (parent >= 0) {
                jmp[v][k] = jmp[parent][k - 1];
                min_cost[v][k] = min(min_cost[v][k - 1], min_cost[parent][k - 1]);
            } else {
                jmp[v][k] = 0;
                min_cost[v][k] = min_cost[v][k - 1];
            }
        }
    }
}

/**
 *
 * @param d массив глубины.
 * @param deep log(n)
 * @param jmp массив прыжков.
 * @param min_cost минимальные стоимости при соответствующем прыжке.
 * @param u вершина.
 * @param v вершина.
 * @return lca(u,v).
 */
int Lca(const int *d, int deep, int **jmp, int **min_cost, int u, int v) {
    if (d[u] < d[v]) {
        std::swap(u, v);
    }
    int delta = d[u] - d[v];
    int res = INF;
    for (int k = deep - 1; k >= 0; k--) {
        auto k2 = 1 << k;
        if (delta >= k2) {
            res = min(res, min_cost[u][k]);
            u = jmp[u][k];
            delta -= k2;
        }
    }
    if (u == v)
        return res;
    res = min(res, min_cost[u][0]);
    res = min(res, min_cost[v][0]);

    for (int k = deep - 1; k >= 0; k--) {
        int u_ = jmp[u][k];
        int v_ = jmp[v][k];
        if (u_ != v_) {
            res = min(res, min_cost[u][k]);
            u = u_;
            res = min(res, min_cost[v][k]);
            v = v_;
        }
    }
    res = min(res, min_cost[u][0]);
    res = min(res, min_cost[v][0]);
    return res;
}

int main() {
    ifstream fin("minonpath.in");
    ofstream fout("minonpath.out");

    int n;
    fin >> n;
    int *p = new int[n]; // дерево заданое предками.
    int *cost = new int[n]; // стоимость перехода к предку.
    int *d = new int[n]; // глубины
    int h_max = 1;
    int h2 = 1;
    int deep = 1;
    p[0] = 0;
    cost[0] = INF;
    d[0] = 1;
    for (int i = 1; i < n; ++i) {
        fin >> p[i] >> cost[i];
        p[i]--;
        d[i] = d[p[i]] + 1; // заполняем сразу массив глубин.
        if (d[i] > deep) {
            deep = d[i];
            while (h2 <= deep) {
                h2 = h2 << 1;
                h_max++;       // и считаем log(n)
            }
        }
    }
    int **jmp = new int *[n]; // массив прыжков
    int **min_cost = new int *[n]; // мин стоимости
    for (int i = 0; i < n; ++i) {
        jmp[i] = new int[h_max];
        min_cost[i] = new int[h_max];
    }

    Precalc(n, p, cost, h_max, jmp, min_cost);
    int m;
    fin >> m;
    for (int i = 0; i < m; ++i) {
        int u, v;
        fin >> u >> v;
        fout << Lca(d, h_max, jmp, min_cost, u - 1, v - 1) << endl;
    }

    for (int i = 0; i < n; ++i) {
        delete[]jmp[i];
        delete[]min_cost[i];
    }
    delete[] jmp; // глубины
    delete[] min_cost; // мин стоимости
    delete[] p;
    delete[] cost;
    delete[] d;
    return 0;
}