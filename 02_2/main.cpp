/*
 * Введенский Вячеслав В. MADE-ML-22
 *
B. Удаление скобок 2.0

Дана строка, составленная из круглых, квадратных и фигурных скобок. Определите, какое наименьшее количество
символов необходимо удалить из этой строки, чтобы оставшиеся символы образовывали правильную скобочную последовательность.

Входные данные
Во входном файле записана строка из круглых, квадратных и фигурных скобок. Длина строки не превосходит {100} символов.
Выходные данные
Выведите строку максимальной длины, являющейся правильной скобочной последовательностью, которую можно получить из
исходной строки удалением некоторых символов.
 */
#include <iostream>
#include <vector>

using namespace std;
/**
 * Некоторое максимальное значение.
 */
const int MAX_VALUE = 1000000000;

/**
 * Поиск подстроки с правильной скобочной подпоследовательностью.
 * @param s строка.
 * @return подстрока.
 */
string solve_subseq(const string &s) {
    size_t n = s.size();
    vector<vector<int>> dp(n);
    vector<vector<string>> sub_sq(n);
    for (int i = 0; i < n; ++i) {
        dp[i] = vector<int>(n, 0);
        sub_sq[i] = vector<string>(n);
    }

    for (int right = 0; right < n; ++right) {
        dp[right][right] = 1;
        sub_sq[right][right] = "";
        for (int left = right - 1; left >= 0; --left) {
            int new_value = MAX_VALUE;
            string new_ss;
            if ((s[left] == '[' && s[right] == ']') ||
                (s[left] == '{' && s[right] == '}') ||
                (s[left] == '(' && s[right] == ')')) {
                new_value = dp[left + 1][right - 1];
                new_ss = s[left] + sub_sq[left + 1][right - 1] + s[right];
            }
            for (int i = left; i < right; ++i) {
                if (dp[left][i] + dp[i + 1][right] < new_value) {
                    new_value = dp[left][i] + dp[i + 1][right];
                    new_ss = sub_sq[left][i] + sub_sq[i + 1][right];
                }
            }
            dp[left][right] = new_value;
            sub_sq[left][right] = new_ss;
        }
    }
    return sub_sq[0][n - 1];
}

int main() {
    string s;
    cin >> s;

    cout << solve_subseq(s);
    return 0;
}
