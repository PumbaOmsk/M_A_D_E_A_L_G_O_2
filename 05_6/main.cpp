/**
 * Введенский Вячеслав В. MADE-ML-22
 *
F. RMQ наоборот
rmq.in, rmq.out
Рассмотрим массив a[1..n]. Пусть Q(i, j) — ответ на запрос о нахождении минимума среди чисел a[i], ..., a[j].
 Вам даны несколько запросов и ответы на них. Восстановите исходный массив.
Входные данные: Первая строка входного файла содержит число n — размер массива, и m — число запросов (1 ≤ n, m ≤ 100 000).
 Следующие m строк содержат по три целых числа i, j и q, означающих, что Q(i, j) = q (1 ≤ i ≤ j ≤ n,  - 2^31 ≤ q ≤ 2^31 - 1).
Выходные данные: Если искомого массива не существует, выведите строку «inconsistent».
В противном случае в первую строку выходного файла выведите «consistent». Во вторую строку выходного файла выведите
 элементы массива. Элементами массива должны быть целые числа в интервале от  - 2^31 до 2^31 - 1 включительно.
 Если решений несколько, выведите любое.
 */
#include <vector>
#include <algorithm>
#include <fstream>
#include <set>

using namespace std;
/// Максимальный элемент массива.
const long long MAX_VALUE = (1LL << 31) - 1;
/// Максимальный элемент для таблицы.
const long long MOD = 1LL << 31;
/// запрос
struct Query {
    int l = 0;
    int r = 0;
    long long v = 0;
};

/// компараторы, для запросов
/// по убыванию значения.
struct ComparatorValue {
    bool operator()(const Query &e1, const Query &e2) const {
        return e1.v > e2.v;
    }
};

/// по возрастанию левой границы
struct ComparatorLeft {
    bool operator()(const Query &e1, const Query &e2) const {
        return e1.l < e2.l;
    }
};

/// по возрастанию правой границы
struct ComparatorRight {
    bool operator()(const Query &e1, const Query &e2) const {
        return e1.r < e2.r;
    }
};

/// заполнение массива элементов по запросам.
/// всегда заполняем максимально возможным элементом.
void FillA(const vector<Query> &q, int n, long long *a) {
    multiset<Query, ComparatorValue> queries;
    multiset<Query, ComparatorLeft> left_queries;
    multiset<Query, ComparatorRight> right_queries;
    for (auto &&query:q) {
        left_queries.emplace(query);
        right_queries.emplace(query);
    }
    for (int i = 0; i < n; ++i) {
        if (right_queries.empty() && left_queries.empty()) {
            break;
        }
        if (right_queries.begin()->r < i) {
            while (!right_queries.empty() && i == right_queries.begin()->r + 1) {
                queries.erase(queries.find(*right_queries.begin()));
                right_queries.erase(right_queries.begin());
            }
        }
        if (i >= left_queries.begin()->l) {
            while (!left_queries.empty() && i == left_queries.begin()->l) {
                queries.emplace(*left_queries.begin());
                left_queries.erase(left_queries.begin());
            }
        }
        if (!queries.empty()) {
            a[i] = queries.begin()->v;
        }
    }
}

/**
 * Считаем SparseTable
 * @param a вектор элементов.
 * @return разряженая таблица.
 */
void FillTable(const long long *a, long long *table, int n, int k_max) {
    for (int i = 0; i < n; ++i) {
        table[i + n * 0] = a[i];
    }
    for (int k = 1; k < k_max; ++k) {
        long k2 = 1 << (k - 1);
        int tdx = n * k;
        int dx = tdx - n;
        for (int l = 0; l < n; ++l) {
            long long m2 = MOD;
            if (l + k2 < n)
                m2 = table[l + k2 + dx];
            table[l + tdx] = min(table[l + dx], m2);
        }
    }
}

/**
 * @param table разряженая таблица.
 * @param u начало отрезка.
 * @param v конец отрезка.
 * @return min на отрезке.
 */
long long Rmq(const long long *table, int n, long u, long v) {
    long l, r;
    if (u < v) {
        l = u;
        r = v;
    } else {
        l = v;
        r = u;
    }
    long k = 1;
    long k2 = 2;
    while (k2 < r - l + 1) {
        k++;
        k2 = k2 << 1;
    }
    k--;
    k2 = k2 >> 1;
    auto m1 = table[l + n * k];
    auto m2 = table[r - k2 + 1 + n * k];
    return min(m1, m2);
}

/// проверка консистентности заполненного массива A списку запросов Q
/// для этого строим SparseTable, пытаемся повторить запрос и сравниваем ответ.
bool Check(const long long *a, int n, const vector<Query> &q, int m) {
    long k_max = 1;
    long temp = 2;
    while (temp < n) {
        temp = temp << 1;
        k_max++;
    }
    auto *table = new long long[n * k_max];
    FillTable(a, table, n, k_max);
    bool consistent = true;
    for (int i = 0; i < m; ++i) {
        auto answer = Rmq(table, n, (long) q[i].l, (long) q[i].r);
        if (answer != q[i].v) {
            consistent = false;
            break;
        }
    }
    delete[]table;
    return consistent;
}


int main() {
    int n, m;
    ifstream fin("rmq.in");
    ofstream fout("rmq.out");

    fin >> n >> m;
    vector<Query> q(m);
    for (int i = 0; i < m; ++i) {
        fin >> q[i].l >> q[i].r >> q[i].v;
        q[i].l--;
        q[i].r--;
    }
    auto *a = new long long[n];
    fill(&a[0], &a[0] + n, MAX_VALUE);

    FillA(q, n, a);
    auto consistent = Check(a, n, q, m);

    if (consistent) {
        fout << "consistent" << "\n";
        for (int i = 0; i < n; ++i) {
            fout << a[i] << " ";
        }
    } else {
        fout << "inconsistent" << "\n";
    }
    fout << flush;
    delete[] a;
    return 0;
}