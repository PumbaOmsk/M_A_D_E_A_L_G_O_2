/*
 * Введенский Вячеслав В. MADE-ML-22
 *
E. Замощение доминошками
Вам дана прямоугольная сетка из квадратных ячеек.
Символ 'X' представляет собой уже покрытую ячейку, символ '.' - это ячейка, которую еще нужно покрыть.
Вы хотите покрыть все ячейки '.', непересекающимися доминошками 2×1
Найдите количество способов сделать это. Два способа считаются различными, если две ячейки покрыты
 одним и тем же домино в одном замощении и двумя разными домино в другом замощении.

 Входные данные
Первая строка содержит числа n и m (1≤n,m≤12). Следующие n строк содержат по m символов — описание доски.

 Выходные данные
Выведите число замощений доминошками.
 */
#include <iostream>
#include <vector>
#include <stack>


using namespace std;

/**
 * @param mask число
 * @param i номер бита.
 * @return значение бита в числе.
 */
int GetBit(int mask, int i) {
    return (mask >> i) & 1;
}

/**
 * Проверка, что ячейка с узаканной позицией - пустая.
 * @param field поле.
 * @param pos позиция.
 * @return true, если ячейка пустая.
 */
bool IsCellEmpty(const vector<vector<int>> &field, int pos) {
    size_t n = field.size();
    size_t m = field[0].size();
    if (pos < 0 || pos > m * n)
        return false;
    int x = static_cast<int>(pos / n);
    int y = static_cast<int>(pos % n);
    return field[y][x] == 0;
}

/**
 * Откорректировать маску в соответствии со следующими клетками поля.
 * @param field поле.
 * @param offset смещение.
 * @param mask маска.
 * @return скорректированная маска.
 */
int MaskFromField(const vector<vector<int>> &field, int offset, int mask) {
    size_t n = field.size();
    size_t mn = field[0].size() * n;
    for (int i = 0; i < n; ++i) {
        if (i + offset < mn && !IsCellEmpty(field, i + offset)) {
            mask |= 1 << i;
        }
    }
    return mask;
}

/**
 * Проверка невалидности маски при условии, что впереди могут быть "Х"
 * @param field поле.
 * @param offset смещение от начала.
 * @param mask маска.
 * @return true, если маска невалидна.
 */
bool InvalidFieldMask(const vector<vector<int>> &field, int offset, int mask) {
    size_t n = field.size();
    size_t mn = field[0].size() * n;
    for (int i = 0; i < n; ++i) {
        if (i + offset < mn && !IsCellEmpty(field, i + offset)) {
            if ((mask & (1 << i)) == 0)
                return true;
        }
    }
    return false;
}

/**
 * Является ли текущая позиция последней в столбце.
 * @param pos позиция.
 * @param n количество строк.
 * @return true, если ячейка последняя
 */
bool isLast(int pos, size_t n) {
    int y = static_cast<int>(pos % n);
    return y == n - 1;
}

/**
 * Подсчёт количества раскладок домино.
 * @param n количество строк.
 * @param m количество столбцов.
 * @param field поле.
 * @return количество вариантов раскладки.
 */
long long int SolveDomino(size_t n, size_t m, const vector<vector<int>> &field) {
    int size = 1 << n;
    auto nm = n * m;
    vector<vector<long long int>> dp(nm + 1, vector<long long int>(size, 0));
    dp[0][MaskFromField(field, 0, 0)] = 1;
    for (int i = 0; i < nm; ++i) { // кол-во прошлых заполненых клеток
        for (int mask = 0; mask < size; ++mask) {
            if (InvalidFieldMask(field, i, mask)) // если маска не соответствует полю
                continue;
            if (!IsCellEmpty(field, i) || GetBit(mask, 0) == 1) { // если следующая ячейка занята и не будем размещать
                auto new_mask = MaskFromField(field, i + 1, mask >> 1);
                dp[i + 1][new_mask] += dp[i][mask];
                continue;
            }
            if (IsCellEmpty(field, i) && IsCellEmpty(field, int(n) + i) &&
                GetBit(mask, 0) == 0) { // пробуем разместить горизонтальную
                auto new_mask = MaskFromField(field, i + 1, (mask >> 1) + (1 << (n - 1)));
                dp[i + 1][new_mask] += dp[i][mask];
            }
            if (IsCellEmpty(field, i) && !isLast(i, n) && IsCellEmpty(field, i + 1) && GetBit(mask, 0) == 0 &&
                GetBit(mask, 1) == 0) { // пробуем разместить вертикальную
                auto new_mask = MaskFromField(field, i + 1, (mask >> 1) + 1);
                dp[i + 1][new_mask] += dp[i][mask];
            }

        }
    }
    return dp[nm][0];
}

int main() {
    size_t n, m;
    cin >> n >> m;
    vector<vector<int>> field(n, vector<int>(m, 0));
    char c;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> c;
            if (c == 'X') {
                field[i][j] = 1;
            }
        }
    }
    cout << SolveDomino(n, m, field);
    return 0;
}
