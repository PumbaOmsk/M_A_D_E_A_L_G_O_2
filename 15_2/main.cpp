/**
 * Введенский Вячеслав В. MADE-ML-22
 *
B. Персистентный массив
 */
#include <iostream>
#include <vector>
#include <random>

using namespace std;

struct Node {
    Node *left, *right;
    int value;
};

struct FPTree {
    FPTree(const vector<int> &arr) {
        max_size = static_cast<int>(arr.size());
        Node *root = build(arr, 0, max_size);
        versions.push_back(root);
    }

    ~FPTree() {
        for (auto &element : elements) {
            delete element;
        }
    }

    Node *build(const vector<int> &arr, int l, int r) {
        Node *node = newNode();
        if (l == r) {
            node->value = arr[l];
        } else {
            int m = (l + r) >> 1;
            node->left = build(arr, l, m);
            node->right = build(arr, m + 1, r);
        }
        return node;
    }

    Node *newNode() {
        Node *node = new Node({nullptr, nullptr, 0});
        elements.push_back(node);
        return node;
    }

    int max_size;            // размер массива
    vector<Node *> versions; // указатели на корни деревьев.
    vector<Node *> elements; // все вершины девевьев - для корректного удаления.


    void set(int ver, int idx, int x) {
        Node *node = set(versions[ver], 0, max_size, idx, x);
        versions.push_back(node);
    }

    int get(int ver, int idx) {
        return get(versions[ver], 0, max_size, idx);
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

};

int main() {
    int n;
    cin >> n;
    vector<int> arr(n);
    for (int k = 0; k < n; ++k) {
        cin >> arr[k];
    }
    FPTree tree(arr);
    int m;
    cin >> m;
    for (int k = 0; k < m; ++k) {
        string command;
        int i, j;
        cin >> command >> i >> j;
        if (command == "get") {
            cout << tree.get(i - 1, j - 1) << "\n";
        } else {
            int x;
            cin >> x;
            tree.set(i - 1, j - 1, x);
        }
    }
    cout.flush();
    return 0;
}