/*
 * Введенский Вячеслав В. MADE-ML-22
 *
 *
E. Кафе

Около Петиного университета недавно открылось новое кафе, в котором действует следующая система скидок:
 при каждой покупке более чем на 100 рублей покупатель получает купон, дающий право на один бесплатный обед
 (при покупке на сумму 100 рублей и меньше такой купон покупатель не получает).

Однажды Пете на глаза попался прейскурант на ближайшие n дней.
 Внимательно его изучив, он решил, что будет обедать в этом кафе все n дней,
 причем каждый день он будет покупать в кафе ровно один обед.
 Однако стипендия у Пети небольшая, и поэтому он хочет по максимуму использовать предоставляемую систему скидок так,
 чтобы его суммарные затраты были минимальны. Требуется найти минимально возможную суммарную стоимость обедов и
 номера дней, в которые Пете следует воспользоваться купонами.

 Входные данные
В первой строке входного файла записано целое число n (0 ≤ n ≤ 100).
 В каждой из последующих n строк записано одно целое число, обозначающее стоимость обеда в рублях
 на соответствующий день. Стоимость — неотрицательное целое число, не превосходящее 300.

Выходные данные
В первой строке выдайте минимальную возможную суммарную стоимость обедов.
 Во второй строке выдайте два числа k1 и k2 — количество купонов, которые останутся неиспользованными
 у Пети после этих n дней и количество использованных им купонов соответственно.

В последующих k2 строках выдайте в возрастающем порядке номера дней, когда Пете следует воспользоваться купонами.
 Если существует несколько решений с минимальной суммарной стоимостью, то выдайте то из них, в котором
 значение k1 максимально (на случай, если Петя когда-нибудь ещё решит заглянуть в это кафе).
 Если таких решений несколько, выведите любое из них.
 * */

#include <memory.h>
#include <stack>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

/**
 * Неопределенное значение.
 */
const int UNKNOWN_VALUE = 1000000000;
/**
 * Граница стоимости после которой дают купон.
 */
const int BOUND = 100;

/**
 * Пересчитать текущую минимальную стоимость для ячейки матрицы.
 * @param i номер строки (количество оставшихся купонов).
 * @param j номер столбца (текущий день).
 * @param cost массив стоимостей обедов.
 * @param dp массив минимальных стоимостей.
 * @return пара (минимальная стоимость обедов, каким прошлым действием пришли к этой ситуации)
 */
pair<int, int> calc_price(int i, int j, const vector<int> &cost, int **dp) {
    if (cost[j] > BOUND) {
        if (j == 0) {
            if (i == 1)
                return make_pair(cost[j], UNKNOWN_VALUE);
            return make_pair(UNKNOWN_VALUE, UNKNOWN_VALUE);
        }
        if (i == 0) {
            if (dp[i + 1][j - 1] != UNKNOWN_VALUE) {
                return make_pair(dp[i + 1][j - 1], -1);
            }
            return make_pair(UNKNOWN_VALUE, UNKNOWN_VALUE);
        }
        int d1 = dp[i - 1][j - 1] != UNKNOWN_VALUE ? dp[i - 1][j - 1] + cost[j] : UNKNOWN_VALUE;
        int d2 = dp[i + 1][j - 1] != UNKNOWN_VALUE ? dp[i + 1][j - 1] : UNKNOWN_VALUE;
        if (d1 > d2)
            return make_pair(d2, -1);
        else
            return make_pair(d1, 1);

    } else {
        if (j == 0) {
            if (i == 0)
                return make_pair(cost[j], UNKNOWN_VALUE);
            return make_pair(UNKNOWN_VALUE, UNKNOWN_VALUE);
        }
        int d1 = dp[i][j - 1] != UNKNOWN_VALUE ? dp[i][j - 1] + cost[j] : UNKNOWN_VALUE;
        int d2 = dp[i + 1][j - 1] != UNKNOWN_VALUE ? dp[i + 1][j - 1] : UNKNOWN_VALUE;
        if (d1 > d2)
            return make_pair(d2, -1);
        else
            return make_pair(d1, 0);
    }
}

/**
 * Восстановить дни когда тратить купоны.
 * @param p направления переходов по таблице (копили или тратили купоны).
 * @param i номер строки.
 * @param j номер столбца.
 * @return дни, когда тратить купоны (с 0).
 */
stack<int> restore_path(int **p, int i, int j) {
    stack<int> path;
    for (; j >= 0; j--) {
        if (p[i][j] == -1)
            path.push(j);
        i -= p[i][j];
    }
    return path;
}

/**
 * Решение задачи покупки обедов по минимальной цене.
 * @param n количество дней.
 * @param cost стоимость обедов.
 * @return пара ((минимальная стоиомсть, количество оставшихся купонов), номера дней когда тратить купоны (с 0))
 */
pair<pair<int, int>, stack<int>> solve_min_price(size_t n, const vector<int> &cost) {
    int m = 1;
    for (int i = 0; i < n; ++i) {
        if (cost[i] > BOUND)
            m++;
    }
    int **p = new int *[m + 1];
    int **dp = new int *[m + 1];
    for (int i = 0; i <= m; ++i) {
        dp[i] = new int[n];
        p[i] = new int[n];

        fill(&dp[i][0], &dp[i][0] + n, UNKNOWN_VALUE);
        fill(&p[i][0], &p[i][0] + n, UNKNOWN_VALUE);
    }

    for (int j = 0; j < n; ++j) {
        for (int i = 0; i < m; ++i) {
            auto res = calc_price(i, j, cost, dp);
            dp[i][j] = res.first;
            p[i][j] = res.second;
        }
    }
    int mx = UNKNOWN_VALUE;
    int imx = -1;
    for (int i = 0; i < m; ++i) {
        if (dp[i][n - 1] <= mx) {
            mx = dp[i][n - 1];
            imx = i;
        }
    }
    int res = dp[imx][n - 1];
    auto path = restore_path(p, imx, static_cast<int>(n) - 1);

    for (int i = 0; i < m; ++i) {
        delete[] dp[i];
        delete[] p[i];
    }
    delete[] dp;
    delete[] p;
    return make_pair(make_pair(res, imx), path);
}

int main() {
    size_t n;

    cin >> n;
    if (n == 0) {
        cout << 0 << endl;
        cout << 0 << " " << 0 << endl;
        return 0;
    }

    vector<int> cost(n);
    for (int i = 0; i < n; ++i) {
        cin >> cost[i];
    }

    auto result = solve_min_price(n, cost);
    cout << result.first.first << endl;
    cout << result.first.second << " " << result.second.size() << endl;
    while (!result.second.empty()) {
        cout << result.second.top() + 1 << endl;
        result.second.pop();
    }
    return 0;
}