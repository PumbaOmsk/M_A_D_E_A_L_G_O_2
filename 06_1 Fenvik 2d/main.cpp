/**
 * Введенский Вячеслав В. MADE-ML-22
 *
A. Звезды
Вася любит наблюдать за звездами. Но следить за всем небом сразу ему тяжело. Поэтому он наблюдает только за частью
 пространства, ограниченной кубом размером n × n × n. Этот куб поделен на маленькие кубики размером 1 × 1 × 1.
 Во время его наблюдений могут происходить следующие события:
    В каком-то кубике появляются или исчезают несколько звезд.
    Петя может поинтересоваться, сколько видно звезд в части пространства, состоящей из нескольких кубиков.
Входные данные: Первая строка входного файла содержит натуральное число 1 ≤ n ≤ 128. Координаты кубиков —
 целые числа от 0 до n - 1. Далее следуют записи о происходивших событиях по одной в строке.
 В начале строки записано число m. Если m равно:
    1, то за ним следуют 4 числа — x, y, z (0 ≤ x, y, z < N) и k ( - 20000 ≤ k ≤ 20000) — координаты кубика и величина,
    на которую в нем изменилось количество видимых звезд;
    2, то за ним следуют 6 чисел — x1, y1, z1, x2, y2, z2 (0 ≤ x1 ≤ x2 < N, 0 ≤ y1 ≤ y2 < N, 0 ≤ z1 ≤ z2 < N), которые означают,
    что Петя попросил подсчитать количество звезд в кубиках (x, y, z) из области: x1 ≤ x ≤ x2, y1 ≤ y ≤ y2, z1 ≤ z ≤ z2;
    3, то это означает, что Васе надоело наблюдать за звездами и отвечать на вопросы Пети.
    Эта запись встречается во входном файле только один раз и будет последней.
Количество записей во входном файле не больше 100 002.
Выходные данные: Для каждого Петиного вопроса выведите искомое количество звезд.
 */
#include <iostream>
#include <vector>
#include <cstring>

using namespace std;
using matrix = long long (*)[128][128];

/**
 * @param i параметр.
 * @return i & (i+1)
 */
long funcF(long i) {
    auto res = i & (i + 1);
    if (res > i)
        return 0;
    return res;
}

/**
 * @param T дерево.
 * @return элемент по индексу.
 */
long long get(const matrix &T, int i, int j, int k) {
    long long res = 0;
    if (i < 0 || j < 0 || k < 0)
        return 0;
    while (i >= 0) {
        int j_ = j;
        while (j_ >= 0) {
            int k_ = k;
            while (k_ >= 0) {
                res += T[i][j_][k_];
                k_ = funcF(k_) - 1;
            }
            j_ = funcF(j_) - 1;
        }
        i = funcF(i) - 1;
    }
    return res;
}

/**
 * Посчитать сумму
 * @param T дерево.
 * @return сумма элементов.
 */
long long rsq(const matrix &T, int x1, int y1, int z1, int x2, int y2, int z2) {
    auto b8 = get(T, x2, y2, z2);
    auto b7 = get(T, x1 - 1, y2, z2);
    auto b6 = get(T, x2, y1 - 1, z2);
    auto b5 = get(T, x1 - 1, y1 - 1, z2);
    auto b4 = get(T, x2, y2, z1 - 1);
    auto b3 = get(T, x1 - 1, y2, z1 - 1);
    auto b2 = get(T, x2, y1 - 1, z1 - 1);
    auto b1 = get(T, x1 - 1, y1 - 1, z1 - 1);
    return b8 - b7 - b6 - b4 + b5 + b3 + b2 - b1;
}

/**
 * Добавить к i-му элементу dx.
 * @param T вектор дерева.
 * @param i индекс.
 * @param dx дельта.
 */
void add(matrix &T, int n, int i, int j, int k, long dx) {
    auto x = i;
    while (x < n) {
        auto y = j;
        while (y < n) {
            auto z = k;
            while (z < n) {
                T[x][y][z] += dx;
                z = z | (z + 1);
            }
            y = y | (y + 1);
        }
        x = x | (x + 1);
    }
}

int main() {
    int n_elem = 128 * 128 * 128;
    auto *memT = new long long[n_elem];
    fill(&memT[0], &memT[n_elem], 0);
    int n;
    cin >> n;
    auto T = (matrix) memT;
    for (int i = 0; i < 100002; ++i) {
        int command;
        cin >> command;
        if (command == 3) {
            break;
        } else if (command == 1) {
            int x, y, z, k;
            cin >> x >> y >> z >> k;
            add(T, n, x, y, z, k);
        } else {
            int x1, y1, z1, x2, y2, z2;
            cin >> x1 >> y1 >> z1 >> x2 >> y2 >> z2;
            cout << rsq(T, x1, y1, z1, x2, y2, z2) << endl;
        }
    }
    delete[] memT;
    return 0;
}