/**
 * Введенский Вячеслав В. MADE-ML-22
 *
Дан массив строк s i и строка t. Требуется для каждой строки s i определить, встречается ли она в t как подстрока.
 */

#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/// отладочная печать
void print(int *arr, int n) {
    cout << "[";
    for (int i = 0; i < n; ++i) { cout << arr[i] << ","; }
    cout << "]" << endl;
}

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

/// проверка в каком отношении находятся подстрока s и суффикс, начинающийся с t[start]
/// -1,0,1  меньше, равен, больше
int lessThan(const string &s, const string &t, int start) {
    int n = t.length();
    for (int i = 0; i < s.length(); ++i) {
        if (s[i] < t[(start + i) % n]) {
            return -1;
        } else if (s[i] > t[(start + i) % n]) {
            return 1;
        }
    }
    return 0;
}

/// проверка есть ли подстрока
bool findSubString(const string &s, const string &t, int *p) {
    int tn = t.length();
    int l = 0;
    int r = tn;
    int idx = (l + r) / 2;
    while (l <= r) {
        auto cmp = lessThan(s, t, p[idx]);
        if (cmp < 0) {
            r = idx - 1;
        } else if (cmp > 0) {
            l = idx + 1;
        } else {
            return true;
        }
        idx = (l + r) / 2;
    }
    return false;
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    int n;
    cin >> n;
    vector<string> lines(n);
    string t;
    for (int i = 0; i < n; ++i) {
        cin >> lines[i];
    }
    string temp;
    cin >> temp;
    t = temp + "$";
    auto sz = t.length();
    int p[sz];
    int pos[sz];
    suffixArray(t, p, pos);

    for (int i = 0; i < n; ++i) {
        if (findSubString(lines[i], t, p))
            cout << "YES\n";
        else
            cout << "NO\n";
    }
    cout << endl;
    return 0;
}