/*
 * Введенский Вячеслав В. MADE-ML-22
 *

 B. Черепаха и монеты
Черепаха хочет переползти из левого верхнего угла поля размером N на M клеток (2 ≤ N, M ≤ 1000) в правый нижний.
 За один шаг она может переместиться на соседнюю клетку вправо или на соседнюю клетку вниз.
 Кроме того, проходя через каждую клетку, Черепаха получает (или теряет)
 несколько золотых монет (это число известно для каждой клетки).
Определите, какое максимальное количество монет может собрать Черепаха по пути и как ей нужно идти для этого.

 Входные данные
В первой строке вводятся два натуральных числа: N и M (2 ≤ N, M ≤ 1000), разделённые пробелом.
 В каждой из следующих N строк записаны через пробел по M чисел aij(|aij| ≤ 10),
 которые обозначают количество монет, получаемых Черепашкой при проходе через каждую клетку.
 Если это число отрицательное, Черепашка теряет монеты.

 Выходные данные
В первой строке программа должна вывести наибольшее количество монет, которое может собрать Черепаха.
 Во второй строке без пробелов выводятся команды, которые нужно выполнить Черепахе:
 буква 'R' (от слова right) обозначает шаг вправо,
 а буква 'D' (от слова down) – шаг вниз.
 */
#include <memory.h>
#include <stack>
#include <iostream>

using namespace std;

/**
 * Неопределенное значение.
 */
const int UNKNOWN_VALUE = -1000000000;

/**
 * Посчитать максимальный доход при переходе на клетку (i,j).
 * @param i номер строки.
 * @param j номер столбца.
 * @param dp максимальный доход при переходе на клетки.
 * @return (максимальный доход при переходе на клетку (i,j), направление откуда пришли).
 */
pair<int, char> calc_income(int i, int j, int **dp) {
    if (i == 0 && j == 0) {
        return std::make_pair(0, 'E');
    }
    if (i == 0) {
        return std::make_pair(dp[i][j - 1], 'R');
    }
    if (j == 0) {
        return std::make_pair(dp[i - 1][j], 'D');
    }

    if (dp[i][j - 1] > dp[i - 1][j]) {
        return std::make_pair(dp[i][j - 1], 'R');
    }
    return std::make_pair(dp[i - 1][j], 'D');
}

/**
 * Восстановить обратную последовательность ходов черепахи.
 * @param p массив, p[i][j] - откуда перешли на клетку (i,j).
 * @param n количество строк.
 * @param m количество столбцов.
 */
stack<char> reverse_jump(char **p, size_t n, size_t m) {
    stack<char> path;
    int i = static_cast<int>(n - 1);
    int j = static_cast<int>(m - 1);
    while (p[i][j] != 'E') {
        path.push(p[i][j]);
        switch (p[i][j]) {
            case 'D':
                i--;
                break;
            case 'R':
                j--;
                break;
            default:
                break;
        }
    }
    return path;
}


/**
 * Решение задачи переходов по клеткам с максимизацией сбора монет.
 * @param n количество строк.
 * @param m количество столбцов.
 * @param income количество монет, которые получит на клетке (-х - потеряет).
 * @param dp максимальный доход при переходе на клетку i,j.
 * @param p каким ходом пришли на i,j клетку.
 */
pair<int, stack<char>> solve_jump_max_income(size_t n, size_t m, int **income) {
    int **dp = new int *[n];
    char **p = new char *[n];
    for (int i = 0; i < n; ++i) {
        dp[i] = new int[m];
        p[i] = new char[m];
        fill(&dp[i][0], &dp[i][0] + m, UNKNOWN_VALUE);
        fill(&p[i][0], &p[i][0] + m, 0);
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            auto res = calc_income(i, j, dp);
            dp[i][j] = res.first + income[i][j];
            p[i][j] = res.second;
        }
    }
    auto path = reverse_jump(p, n, m);
    int max_income = dp[n - 1][m - 1];

    for (int i = 0; i < n; ++i) {
        delete[] dp[i];
        delete[] p[i];
    }
    delete[] dp;
    delete[] p;

    return make_pair(max_income, path);
}

int main() {
    size_t n, m;
    cin >> n >> m;

    int **income = new int *[n];
    for (int i = 0; i < n; ++i) {
        income[i] = new int[m];
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> income[i][j];
        }
    }

    auto res = solve_jump_max_income(n, m, income);
    auto max_income = res.first;
    auto path = res.second;

    cout << max_income << endl;
    while (!path.empty()) {
        cout << path.top();
        path.pop();
    }

    for (int i = 0; i < n; ++i) {
        delete[] income[i];
    }
    delete[] income;
    return 0;
}