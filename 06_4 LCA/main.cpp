/**
 * Введенский Вячеслав В. MADE-ML-22
 *
D. LCA
Дано подвешенное дерево с корнем в первой вершине. Вам нужно ответить на m запросов вида "найти LCA двух вершин".
Входные данные: В первой строке задано целое число n — число вершин в дереве (1≤n≤2⋅10^5).
В следующих n−1 строках записано одно целое число x. Число x на строке i означает, что x — предок вершины i(x<i).
Затем дано число m Далее заданы m(0≤m≤5⋅105) запросов вида (u,v) — найти LCA двух вершин u и v (1≤u,v≤n; u≠v).
Выходные данные: Для каждого запроса выведите LCA двух вершин на отдельной строке.
 */
#include <iostream>

using namespace std;

/**
 * Предподсчёт массива jmp - куда прыгаем.
 * @param n количество вершин в графе.
 * @param p массив предков.
 * @param deep глубина массива прыжков.
 * @param jmp двумерный массив [n][deep]. [v][k] куда прыгнем из вершины v длиной 2^k.
 */
void Precalc(int n, const int *p, int deep, int **jmp) {
    for (int v = 0; v < n; ++v) {
        jmp[v][0] = p[v];
    }
    for (int k = 1; k < deep; ++k) {
        for (int v = 0; v < n; ++v) {
            auto parent = jmp[v][k - 1];
            if (parent >= 0)
                jmp[v][k] = jmp[parent][k - 1];
            else
                jmp[v][k] = 0;
        }
    }
}

/**
 *
 * @param d массив глубины.
 * @param deep log(n)
 * @param jmp массив прыжков.
 * @param u вершина.
 * @param v вершина.
 * @return lca(u,v).
 */
int Lca(const int *d, int deep, int **jmp, int u, int v) {
    if (d[u] < d[v]) {
        std::swap(u, v);
    }
    int delta = d[u] - d[v];
    for (int k = deep - 1; k >= 0; k--) {
        auto k2 = 1 << k;
        if (delta >= k2) {
            u = jmp[u][k];
            delta -= k2;
        }
    }
    if (u == v)
        return u;
    for (int k = deep - 1; k >= 0; k--) {
        int u_ = jmp[u][k];
        int v_ = jmp[v][k];
        if (u_ != v_) {
            u = u_;
            v = v_;
        }
    }
    return jmp[u][0];
}

int main() {
    int n;
    cin >> n;
    int *p = new int[n]; // дерево заданое предками.
    int *d = new int[n]; // глубины
    int h_max = 0;
    int h2 = 1;
    int deep = 1;
    p[0] = -1;
    d[0] = 1;
    for (int i = 1; i < n; ++i) {
        cin >> p[i];
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
    for (int i = 0; i < n; ++i) {
        jmp[i] = new int[h_max];
    }

    Precalc(n, p, h_max, jmp);
    int m;
    cin >> m;
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        cout << Lca(d, h_max, jmp, u - 1, v - 1) + 1 << endl;
    }


    for (int i = 0; i < n; ++i) {
        delete[]jmp[i];
    }
    delete[] jmp; // глубины
    delete[] p;
    delete[] d;
    return 0;
}