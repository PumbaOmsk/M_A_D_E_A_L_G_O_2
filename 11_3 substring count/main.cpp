/**
 * Введенский Вячеслав В. MADE-ML-22
 *
C. Количество подстрок
 Вычислите количество различных подстрок строки s.
 */
#include <iostream>
#include <utility>

using namespace std;

/**
 * построение суффиксного массива
 * @param s строка с $ на конце
 * @param p суффиксный массив
 * @param pos обратный индекс
 */
void suffixArray(const string &s, int *p, int *pos) {
    int n = static_cast<int>(s.length());
    int sz_count = max(n, 256);
    int c[n], count[sz_count];
    std::fill(&count[0], &count[255], 0);
    for (int i = 0; i < n; ++i)
        ++count[s[i]];
    for (int i = 1; i < 256; ++i)
        count[i] += count[i - 1];
    for (int i = 0; i < n; ++i)
        p[--count[s[i]]] = i;
    c[p[0]] = 0;
    int cl = 1;
    for (int i = 1; i < n; ++i) {
        if (s[p[i]] != s[p[i - 1]]) ++cl;
        c[p[i]] = cl - 1;
    }
    int pn[n];
    int _2k = 1;
    for (int k = 0; _2k < n; ++k, _2k = 1 << k) {
        for (int i = 0; i < n; ++i) {
            pn[i] = p[i] - _2k;
            if (pn[i] < 0) pn[i] += n;
        }
        std::fill(&count[0], &count[cl], 0);
        for (int i = 0; i < n; ++i)
            ++count[c[pn[i]]];
        for (int i = 1; i < cl; ++i)
            count[i] += count[i - 1];
        for (int i = n - 1; i >= 0; --i)
            p[--count[c[pn[i]]]] = pn[i];
        pos[p[0]] = 0;
        cl = 1;
        for (int i = 1; i < n; ++i) {
            int mid1 = (p[i] + _2k) % n;
            int mid2 = (p[i - 1] + _2k) % n;
            if (c[p[i]] != c[p[i - 1]] || c[mid1] != c[mid2])
                ++cl;
            pos[p[i]] = cl - 1;
        }
        std::copy(&pos[0], &pos[n - 1], &c[0]);
    }
}

/**
 * Подсчёт lcp
 * @param s строка с $ на конце
 * @param p суффиксный массив
 * @param pos обратные индексы
 * @param lcp для записи результата
 */
void computeLcp(const string &s, int *p, int *pos, int *lcp) {
    int n = static_cast<int>(s.length());
    int prev = 0;
    lcp[0] = 0;
    for (int i = 0; i < n - 1; ++i) {
        int j = p[pos[i] - 1];
        int cur = prev;
        while (s[(i + cur) % n] == s[(j + cur) % n]) {
            cur++;
        }
        lcp[pos[i]] = cur;
        prev = max(cur - 1, 0);
    }
}

int main() {
    string sin;
    cin >> sin;
    string s = sin + "$";
    unsigned long long n = s.length();
    int p[n];
    int pos[n];
    int lcp[n];
    suffixArray(s, p, pos);
    computeLcp(s, p, pos, lcp);

    unsigned long long res_lcp = 0;
    for (int i = 2; i < n; ++i) {
        res_lcp += lcp[i];
    }
    unsigned long long res = (n) * (n - 1) / 2;
    res -= res_lcp;
    cout << res << endl;
    return 0;
}