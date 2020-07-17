/*
 * Введенский Вячеслав В. MADE-ML-22
 *
C. Наибольшая возрастающая подпоследовательность
Пусть a1, a2, ..., an — числовая последовательность.
 Длина последовательности — это количество элементов этой последовательности.
 Последовательность ai1, ai2, ..., aik называется подпоследовательностью последовательности a,
 если 1 ≤ i1 < i2 < ... < ik ≤ n. Последовательность a называется возрастающей, если a1 < a2 < ... < an.

Вам дана последовательность, содержащая n целых чисел. Найдите ее самую длинную возрастающую подпоследовательность.

 Входные данные
В первой строке задано одно число n (1 ≤ n ≤ 2000) — длина подпоследовательности.
 В следующей строке задано n целых чисел ai ( - 109 ≤ ai ≤ 109) — элементы последовательности.

Выходные данные
В первой строке выведите число k — длину наибольшей возрастающей подпоследовательности.
 В следующей строке выведите k чисел — саму подпоследовательность.
 */

#include <memory.h>
#include <stack>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

/**
 * Посчитать максимальную длину возрастающей подпоследовательности, оканчивающейся в этом элементе.
 * @param i текущий элемент.
 * @param dp значения длин подпоследовательностей.
 * @param sq исходная последовательность.
 * @return пара (максимальная длина, предыдущий элемент).
 */
pair<int, int> calc_sq_len(int i, const vector<int> &dp, const vector<int> &sq) {
    int mx = 0;
    int imx = -1;
    for (int j = 0; j < i; ++j) {
        if (dp[j] > mx && sq[j] < sq[i]) {
            mx = dp[j];
            imx = j;
        }
    }
    return std::make_pair(mx, imx);
}

/**
 * Восстановить подпоследовательность в обратном порядке.
 * @param sq последовательность.
 * @param p массив предыдущих элементов.
 * @param idx конечный элемент подпоследовательности.
 * @return элементы подпоследовательности.
 */
stack<int> reverse_collect(const vector<int> &sq, const vector<int> &p, int idx) {
    stack<int> path;
    while (p[idx] != -1) {
        path.push(sq[idx]);
        idx = p[idx];
    }
    path.push(sq[idx]);
    return path;
}

/**
 * Поиск возростающей подпоследовательности максимальной длины.
 * @param n длина последовательности.
 * @param sequence последовательность.
 * @return элементы подпоследовательности.
 */
stack<int> find_max_sub_sequence(const vector<int> &sequence) {
    size_t n = sequence.size();
    vector<int> dp(n, 0);
    vector<int> p(n, 0);

    int mx = -1;
    int imx = -1;
    for (int i = 0; i < n; ++i) {
        auto res = calc_sq_len(i, dp, sequence);
        dp[i] = res.first + 1;
        p[i] = res.second;
        if (dp[i] > mx) {
            mx = dp[i];
            imx = i;
        }
    }
    auto sub_sequence = reverse_collect(sequence, p, imx);
    return sub_sequence;
}

int main() {
    size_t n;

    cin >> n;
    vector<int> sequence(n);
    for (int i = 0; i < n; ++i) {
        cin >> sequence[i];
    }

    auto sub_sequence = find_max_sub_sequence(sequence);

    cout << sub_sequence.size() << endl;
    while (!sub_sequence.empty()) {
        cout << sub_sequence.top() << " ";
        sub_sequence.pop();
    }

    return 0;
}