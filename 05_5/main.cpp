/**
 * Введенский Вячеслав В. MADE-ML-22
 *
 E. Криптография
crypto.in , crypto.out
Задано n матриц A1, A2, ..., An размера 2 × 2. Необходимо для нескольких запросов вычислить произведение матриц
 Ai, Ai + 1, ..., Aj. Все вычисления производятся по модулю r.
Входные данные: Первая строка входного файла содержит числа r (1 ≤ r ≤ 10 000), n(1 ≤ n ≤ 200 000) и m(1 ≤ m ≤ 200 000).
Следующие n блоков по две строки содержащие по два числа в строке — описания матриц. Затем следуют m пар целых чисел
 от 1 до n, запросы на произведение на отрезке.
Выходные данные: Выведите m блоков по две строки,по два числа в каждой — произведения на отрезках. Разделяйте
блоки пустой строкой. Все вычисления производятся по модулю r
 */
#include <vector>
#include <fstream>

using namespace std;

/**
 * Установить значения m[0..3]
 */
void Set(long *m, long e0, long e1, long e2, long e3) {
    m[0] = e0;
    m[1] = e1;
    m[2] = e2;
    m[3] = e3;
}

/**
 * Умножить матрицу p матрицу a. результат в p по модулю mod.
 */
void Mult(long *p, const long *a, long mod) {
    long b0, b1, b2, b3;
    b0 = (p[0] * a[0] + p[1] * a[2]) % mod;
    b1 = (p[0] * a[1] + p[1] * a[3]) % mod;
    b2 = (p[2] * a[0] + p[3] * a[2]) % mod;
    b3 = (p[2] * a[1] + p[3] * a[3]) % mod;
    Set(p, b0, b1, b2, b3);
}

/**
 * вернуть минимальный элемент на подотрезке.
 * @param a левая граница исходного отрезка.
 * @param b правая граница исходного отрезка.
 * @param res место для результата.
 * @return минимальный элемент.
 */
void Rsq(const long *const T, int v, int l, int r, int a, int b, long mod, long *res) {
    if (l > b || r < a) {
        Set(res, 1, 0, 0, 1);
        return;
    }
    if (l >= a && r <= b) {
        auto idx = v << 2;
        Set(res, T[idx + 0], T[idx + 1], T[idx + 2], T[idx + 3]);
        return;
    }
    int m = (l + r) >> 1;
    long res_temp[8];
    Set(res_temp, 1, 0, 0, 1);
    Set(&res_temp[4], 1, 0, 0, 1);
    auto dx = v << 1;
    Rsq(T, dx + 1, l, m, a, b, mod, res_temp);
    Rsq(T, dx + 2, m + 1, r, a, b, mod, &res_temp[4]);
    Mult(res, &res_temp[0], mod);
    Mult(res, &res_temp[4], mod);
}

int main() {
    long r;
    int n, m;
    ifstream fin("crypto.in");
    ofstream fout("crypto.out");

    fin >> r >> n >> m;

    int bound = 1;
    while (bound < n) {
        bound = bound << 1;
    }
    long long size = 2 * bound - 1;

    long *T;
    T = new long[4 * size];
    std::fill(&T[0], &T[4 * size], 0);

    for (int i = 0; i < size; ++i) {
        T[4 * i + 0] = 1;
        T[4 * i + 3] = 1;
    }
    for (int i = 0; i < n; ++i) {
        fin >> T[4 * (bound - 1 + i) + 0] >> T[4 * (bound - 1 + i) + 1]
            >> T[4 * (bound - 1 + i) + 2] >> T[4 * (bound - 1 + i) + 3];
    }

    for (int v = bound - 2; v >= 0; v--) {
        Mult(&T[4 * v], &T[4 * (2 * v + 1)], r);
        Mult(&T[4 * v], &T[4 * (2 * v + 2)], r);
    }

    for (int row = 0; row < m; ++row) {
        long res[4];
        Set(res, 1, 0, 0, 1);
        long i, j;
        fin >> i >> j;

        Rsq(T, 0, 0, bound - 1, i - 1, j - 1, r, res);
        fout << res[0] << " " << res[1] << "\n";
        fout << res[2] << " " << res[3] << "\n";
        fout << "\n";
    }
    fout << flush;
    delete[]T;
    return 0;
}
