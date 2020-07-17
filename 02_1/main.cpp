/*
 * Введенский Вячеслав В. MADE-ML-22
 *

 A. Разбиение на пары
Необходимо n человек разбить на команды по 2 человека. Не все хорошо ладят друг с другом.
Есть матрица совместимости. Сформировать максимальное число команд.

 Входные данные
Первая строка содержит число n (1 ≤ n ≤ 18), далее n строк по n символов описывают отношения ребят.
 Символ j строки i равен «Y», если ребята i и j могут участвовать в команде.

 Выходные данные
Выведите одно число — максимальное число участников в командах, которые можно сформировать.
 */
#include <iostream>
#include <vector>

using namespace std;

/**
 * @param mask число
 * @param i номер бита.
 * @return значение бита в числе.
 */
int getBit(int mask, int i) {
    return (mask >> i) & 1;
}

/**
 * Поиск паросочетаний.
 * @param n размерность графа.
 * @param compatibility матрица совместимости людей (по сути граф).
 * @return количество человек в максимальном количестве команд.
 */
int solve_pair(size_t n, const vector<vector<int>> &compatibility) {
    auto size = 1 << n;
    vector<signed char> dp(size, 0);
    for (int mask = 0; mask < size; ++mask) {
        for (int i = 0; i < n; ++i) {
            if (getBit(mask, i) == 1) {
                dp[mask] = dp[mask - (1 << i)];
                for (int j = 0; j < n; ++j) {
                    if (getBit(mask, j) == 1 && compatibility[i][j]) {
                        auto newPair = static_cast<signed char>(dp[mask - (1 << i) - (1 << j)] + 1);
                        dp[mask] = max(dp[mask], newPair);
                    }
                }
            }
        }
    }
    return dp[size - 1] * 2;
}

int main() {
    size_t n;
    cin >> n;

    vector<vector<int>> compatibility(n);
    for (int i = 0; i < n; ++i) {
        compatibility[i] = vector<int>(n, 0);
    }

    char c;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> c;
            if (c == 'Y')
                compatibility[i][j] = 1;
        }
    }

    cout << solve_pair(n, compatibility);
    return 0;
}
