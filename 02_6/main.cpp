/*
 * Введенский Вячеслав В. MADE-ML-22
 *
F. Дубы
Можно удалять если слева и справа строго ниже или строго выше. Крайние нельзя.

Требуется план, чтобы осталось максимум и образовывали неубывающую последовательность.

Входные данные
n — количество дубов, растущих на аллее (2 ≤ n ≤ 200).
 Вторая строка содержит n чисел — высоты дубов, приведенные слева направо. Высоты дубов — <=1 000.

 Выходные данные
Если оставить с неубывающими высотами невозможно - - 1.

План
 в первую строку выходного файла выведите целое число m — минимальное количество дубов, которые необходимо срубить.
 В следующие m строк выведите оптимальный план вырубки деревьев — номера дубов в том порядке,
 в котором их следует срубать, по одному номеру на строке.
Дубы нумеруются слева направо натуральными числами от 1 до n.
Если планов с наименьшим числом срубаемых дубов несколько, выведите любой из них.
 */
#include <iostream>
#include <vector>

using namespace std;
/**
 * Некоторое максимальное значение.
 */
const int MAX_VALUE = 1000000000;

/**
 * Можно ли рубить дерево.
 * @param left высота слева.
 * @param center высота дерева.
 * @param right высота справа.
 * @return true, если можно рубить.
 */
bool CanDelete(int left, int center, int right) {
    if (left < center && center > right) return true;
    if (left > center && center < right) return true;
    return false;
}

/**
 * Рубим деревья
 * @param dp_del номера последних деревьев.
 * @param left левая граница.
 * @param right правая граница.
 * @param plan вектор с планом рубки.
 */
void PushTree(const vector<vector<int>> &dp_del, int left, int right, vector<int> &plan) {
    if (dp_del[left][right] == left) return;
    PushTree(dp_del, left, dp_del[left][right], plan);
    PushTree(dp_del, dp_del[left][right], right, plan);
    plan.push_back(dp_del[left][right]);
}

/**
 * Восстановить план рубки деревьев.
 * @param dp_del номера последних деревьев на отрезках.
 * @param last номера последних удаленных деревьев.
 * @param left левая граница.
 * @param right правая граница.
 * @param plan вектор для складывания последовательности рубки.
 */
void RestorePlan(const vector<vector<int>> &dp_del,
                 const vector<vector<int>> &last,
                 int left, int right,
                 vector<int> &plan) {
    if (last[left][right] == left) {
        PushTree(dp_del, left, right, plan);
        return;
    }
    RestorePlan(dp_del, last, left, last[left][right], plan);
    RestorePlan(dp_del, last, last[left][right], right, plan);
}

/**
 * Строим план рубки деревьев.
 * @param trees высоты деревьев.
 * @return пара (количество срубленых деревьев, вектор с последовательностью рубки)
 */
pair<int, vector<int>> SolvePlan(const vector<int> &trees) {
    size_t n = trees.size();
    vector<vector<int>> dp_del(n, vector<int>(n, -1));     // номер последнего дерева на интервале удаления [i,j)
    vector<vector<int>> dp(n, vector<int>(n, MAX_VALUE));  // минимум удаляемых деревьев
    vector<vector<int>> last(n, vector<int>(n, -1));       // последнее удаленное дерево

    for (int left = 0; left + 1 < n; ++left) {
        dp_del[left][left + 1] = left;
    }
    for (int len = 2; len < n; ++len) { // по длинам отрезков
        for (int left = 0; left + len < n; ++left) { // все варианты отрезков
            int right = left + len;
            for (int i = left + 1; i < right; ++i) { // разбиваем отрезок на 2
                if (dp_del[left][i] >= 0 && dp_del[i][right] >= 0 && CanDelete(trees[left], trees[i], trees[right])) {
                    dp_del[left][right] = i;
                    break;
                }
            }
        }
    }

    for (int len = 1; len < n; ++len) { // по длинам отрезков
        for (int left = 0; left + len < n; ++left) { // все варианты отрезков
            int right = left + len;
            if (trees[left] > trees[right]) {
                continue;
            }
            if (dp_del[left][right] >= 0) {
                dp[left][right] = right - left - 1;
                last[left][right] = left;
            }
            for (int i = left + 1; i < right; ++i) { // разбиваем отрезок на 2
                if (trees[left] > trees[i] || trees[i] > trees[right]) {
                    continue;
                }
                int sum = dp[left][i] + dp[i][right];
                if (dp[left][right] > sum) {
                    dp[left][right] = sum;
                    last[left][right] = i;
                }
            }
        }
    }

    vector<int> plan;
    if (last[0][n - 1] == -1)
        return make_pair(-1, plan);

    RestorePlan(dp_del, last, 0, static_cast<int>(n - 1), plan);
    return make_pair(dp[0][n - 1], plan);
}

int main() {
    size_t n;
    cin >> n;
    vector<int> trees(n, 0);
    for (int i = 0; i < n; ++i) {
        cin >> trees[i];
    }
    auto res = SolvePlan(trees);
    cout << res.first << endl;
    auto plan = res.second;
    for (auto p:plan) {
        cout << p + 1 << endl;
    }
    return 0;
}
