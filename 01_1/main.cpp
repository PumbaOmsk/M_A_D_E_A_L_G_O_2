/*
 * Введенский Вячеслав В. MADE-ML-22
 *

 A. Кузнечик собирает монеты
ввод input.txt
вывод output.txt

Кузнечик прыгает по столбикам, расположенным на одной линии на равных расстояниях друг от друга.
 Столбики имеют порядковые номера от 1 до N. В начале Кузнечик сидит на столбике с номером 1 и хочет добраться
 до столбика с номером N. Он может прыгнуть вперед на расстояние от 1 до K столбиков, считая от текущего.

На каждом столбике Кузнечик может получить или потерять несколько золотых монет (для каждого столбика это число
 известно). Определите, как нужно прыгать Кузнечику, чтобы собрать наибольшее количество золотых монет.
 Учитывайте, что Кузнечик не может прыгать назад.

 Входные данные
В первой строке вводятся два натуральных числа: N и K (2 ≤ N, K ≤ 10000), разделённые пробелом. Во второй строке
 записаны через пробел N - 2 целых числа – количество монет, которое Кузнечик получает на каждом столбике,
 от 2-го до N - 1-го. Если это число отрицательное, Кузнечик теряет монеты. Гарантируется, что все числа по
 модулю не превосходят 10 000.

 Выходные данные
В первой строке программа должна вывести наибольшее количество монет, которое может собрать Кузнечик.
 Во второй строке выводится число прыжков Кузнечика,
 а в третьей строке – номера всех столбиков, которые посетил Кузнечик (через пробел в порядке возрастания).

Если правильных ответов несколько, выведите любой из них.
 */
#include <memory.h>
#include <stack>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

/**
 * Неопределенное значение.
 */
const int UNKNOWN_VALUE = -1000000000;

/**
 * Посчитать максимальный доход при допрыгивании на столбик i.
 * @param i номер текущего столбика.
 * @param k длина прыжка
 * @param dp максимальный доход при допрыгивании до i-го столбика.
 * @return максимальный доход при допрыгивании на столбик i.
 */
pair<int, int> calc_income(int i, int k, const vector<int> &dp) {
    int mx = UNKNOWN_VALUE;
    int imx = UNKNOWN_VALUE;
    for (int j = i - 1; j >= 0 && j >= i - k; j--) {
        if (mx == UNKNOWN_VALUE || dp[j] > mx) {
            mx = dp[j];
            imx = j;
        }
    }
    return std::make_pair(mx, imx);
}

/**
 * Восстановить обратную последовательность прыжков кузнечика.
 * @param p массив, p[i] - откуда прыгнули на i-й столбик.
 * @return последовательность прыжков.
 */
stack<int> reverse_jump(const vector<int> &p) {
    stack<int> path;
    int i = static_cast<int>(p.size()) - 1;
    while (p[i] != -1) {
        path.push(i);
        i = p[i];
    }
    path.push(0);
    return path;
}

/**
 * Решение задачи прыжков по столбикам с максимизацией сбора монет.
 * @param income количество монет, которые получит на столбике (-х - потеряет).
 * @param k  длина прыжка.
 * @return пара (количество собраных монет, последовательность прыжков)
 */
pair<int, stack<int>> solve_jump_max_income(const vector<int> &income, size_t k) {
    size_t n = income.size();
    vector<int> dp(n, UNKNOWN_VALUE);
    vector<int> p(n, UNKNOWN_VALUE);

    dp[0] = 0;
    p[0] = -1;
    for (int i = 1; i < n; ++i) {
        auto res = calc_income(i, static_cast<int>(k), dp);
        dp[i] = res.first + income[i];
        p[i] = res.second;
    }
    return make_pair(dp[n - 1], reverse_jump(p));
}


int main() {
    size_t n, k;

    ifstream file_in("input.txt");
    ofstream file_out("output.txt");

    file_in >> n >> k;
    vector<int> income(n);
    income[0] = income[n - 1] = 0;
    for (int i = 1; i < n - 1; ++i) {
        file_in >> income[i];
    }


    auto res = solve_jump_max_income(income, k);
    auto path = res.second;


    file_out << res.first << endl;
    file_out << path.size() - 1 << endl;
    while (!path.empty()) {
        file_out << path.top() + 1 << " ";
        path.pop();
    }
    file_out.close();

    return 0;
}