/**
 * Введенский Вячеслав В. MADE-ML-22
 *
 K-я порядковая статистика на отрезке
 */

#include <iostream>
#include <vector>
#include <algorithm>

const int MOD = static_cast<const int>(1e9);
using namespace std;

// пара чисел (n,2^n) большее, чем l
pair<int, int> get2n(int l) {
    int p = 0;
    int v = 1;
    while (v < l) {
        v = v << 1;
        p++;
    }
    return make_pair(p, v);
}

struct Node {
    Node *left, *right;
    int value;
};

struct FPTree {
    FPTree(const int *arr, int n) {
        max_size = get2n(n).second;
        Node *root = build(arr, 0, max_size);
        versions.push_back(root);
    }

    FPTree(int n) {
        max_size = get2n(n).second;
        Node *root = build(nullptr, 0, max_size);
        versions.push_back(root);
    }

    ~FPTree() {
        for (auto &element : elements) {
            delete element;
        }
    }

    void set(int ver, int idx, int x) {
        Node *node = set(versions[ver], 0, max_size - 1, idx, x);
        versions.push_back(node);
    }

    void set(int idx, int x) {
        Node *node = set(last(), 0, max_size - 1, idx, x);
        versions.push_back(node);
    }

    int get(int ver, int idx) {
        return get(versions[ver], 0, max_size - 1, idx);
    }

    int get(int idx) {
        return get(last(), 0, max_size - 1, idx);
    }

    int kStat(int l, int r, int k) {
        Node *node_l = versions[l];
        Node *node_r = versions[r + 1];
        return kStat(node_r, node_l, k, 0, max_size - 1);
    }

private:
    int max_size;            // размер массива
    vector<Node *> versions; // указатели на корни деревьев.
    vector<Node *> elements; // все вершины девевьев - для корректного удаления.

    Node *build(const int *arr, int l, int r) {
        Node *node = newNode();
        if (l == r) {
            if (arr == nullptr)
                node->value = 0;
            else
                node->value = arr[l];
        } else {
            int m = (l + r) >> 1;
            node->left = build(arr, l, m);
            node->right = build(arr, m + 1, r);
            node->value = node->left->value + node->right->value;
        }
        return node;
    }

    Node *newNode() {
        Node *node = new Node({nullptr, nullptr, 0});
        elements.push_back(node);
        return node;
    }

    Node *set(Node *current, int l, int r, int idx, int x) {
        Node *node = newNode();
        if (l == r) {
            node->value = x;
        } else {
            int m = (l + r) >> 1;
            if (idx <= m) {
                node->right = current->right;
                node->left = set(current->left, l, m, idx, x);
            } else {
                node->right = set(current->right, m + 1, r, idx, x);
                node->left = current->left;
            }
            node->value = node->left->value + node->right->value;
        }
        return node;
    }

    int get(Node *node, int l, int r, int idx) {
        if (l == r) {
            return node->value;
        } else {
            int m = (l + r) >> 1;
            if (idx <= m)
                return get(node->left, l, m, idx);
            else
                return get(node->right, m + 1, r, idx);
        }
    }

    // параллельный проход по двум деревьям
    int kStat(Node *root_r, Node *root_l, int k, int l, int r) {
        if (l == r) {
            return l;
        }
        int m = (l + r) >> 1;
        int lv = root_r->left->value - root_l->left->value;
        if (lv >= k) {
            return kStat(root_r->left, root_l->left, k, l, m);
        } else {
            return kStat(root_r->right, root_l->right, k - lv, m + 1, r);
        }
    }

    Node *last() {
        return versions[versions.size() - 1];
    }
};


int main() {
    int N; // <=450_000
    cin >> N;
    long long a1, l, m; // < 10^9
    cin >> a1 >> l >> m;
    int *a = new int[N];
    int *sorted = new int[N];
    a[0] = a1;
    sorted[0] = a[0];
    for (int i = 1; i < N; ++i) {
        int a_i = (a[i - 1] * l + m) % MOD;
        a[i] = a_i;
        sorted[i] = a_i;
    }
    sort(sorted, sorted + N);
    for (int i = 0; i < N; ++i) {
        a[i] = lower_bound(sorted, sorted + N, a[i]) - sorted;
    }
    // теперь a[i] из отрезка [0..N-1], а в sorted[i] - исходное число
    FPTree tree(N);
    for (int i = 0; i < N; ++i) {
        int v = tree.get(a[i]);
        tree.set(a[i], 1 + v);
    }

    int b; // количество групп запросов <1000
    cin >> b;
    long long sum = 0; // !!!!!!!!!!
    for (int i = 0; i < b; ++i) {
        int g; // количество запросов в группе
        cin >> g;
        long long x1, lx, mx; // < 10^9
        long long y1, ly, my; //
        long long k1, lk, mk; //
        cin >> x1 >> lx >> mx >> y1 >> ly >> my >> k1 >> lk >> mk;

        int ig = min(x1, y1);
        int jg = max(x1, y1);
        int xg, yg, kg = k1;
        int istat = tree.kStat(ig - 1, jg - 1, kg);
        sum += sorted[istat]; // !!!!!!!!!!!!!!!
        for (int j = 1; j < g; ++j) {
            xg = (((ig - 1) * lx + mx) % N) + 1;
            yg = (((jg - 1) * ly + my) % N) + 1;
            ig = min(xg, yg);
            jg = max(xg, yg);
            kg = (((kg - 1) * lk + mk) % (jg - ig + 1)) + 1;
            istat = tree.kStat(ig - 1, jg - 1, kg);
            sum += sorted[istat]; // !!!!!!!!!!!!!!!
        }
    }
    cout << sum << endl;
    delete[] a;
    delete[] sorted;
    return 0;
}