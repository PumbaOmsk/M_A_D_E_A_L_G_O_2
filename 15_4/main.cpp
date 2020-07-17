#include <memory>

/**
 * Введенский Вячеслав, MADE-22
 *
 */
#include <iostream>
#include <random>
#include <ctime>
#include <cassert>
#include <list>
#include <memory>

using namespace std;
struct TreapNode;
typedef shared_ptr<TreapNode> PtrTreapNode; // пришлось делать на умных указателях. Иначе провал по памяти.

struct TreapNode {
    explicit TreapNode() : value(0), sum(0), size(1), left(nullptr), right(nullptr) {}

    int value;                // значение
    unsigned long long sum;   // сумма поддерева
    int size;                 // размер поддерева.
    PtrTreapNode left;
    PtrTreapNode right;
};

// значение размера дерева.
static int sizeTree(const PtrTreapNode &node) {
    return node == nullptr ? 0 : node->size;
}

// значение размера дерева.
static unsigned long long sumTree(const PtrTreapNode &node) {
    return node == nullptr ? 0 : node->sum;
}

// пересчёт размера дерева, при условии, что дети корректны.
static void update(const PtrTreapNode &node) {
    if (node) {
        node->size = sizeTree(node->left) + sizeTree(node->right) + 1;
        node->sum = sumTree(node->left) + sumTree(node->right) + node->value;
    }
};

// персистентное дерево, но при этом не храним версии.
class FPTreap {
public:
    FPTreap();

    FPTreap(const int *a, int n);

    void print(int l, int r) const;

    unsigned long long sum(int l, int r);

    void copy(int a, int b, int l);

private:
    PtrTreapNode last_root = nullptr;

    PtrTreapNode newNode(int value);

    PtrTreapNode newNode(const PtrTreapNode &);

    PtrTreapNode split(const PtrTreapNode &node, int size, PtrTreapNode &_left, PtrTreapNode &_right);

    PtrTreapNode merge(PtrTreapNode _left, PtrTreapNode _right);

    unsigned long long sum_subtree(const PtrTreapNode &node, int l, int r) const;

    void print_subtree(TreapNode *node) const;

    void print_subtree(TreapNode *node, int l, int r) const;

    PtrTreapNode build(const int *arr, int l, int r);
};

FPTreap::FPTreap() {
    srand(17); // srand(time(nullptr));
}

FPTreap::FPTreap(const int *a, int n) {
    srand(17); // srand(time(nullptr));
    last_root = build(a, 0, n - 1);
}

// начальная инициализация массивом. Сами строим дерево.
PtrTreapNode FPTreap::build(const int *arr, int l, int r) {
    if (r < l || l > r || l < 0 || r < 0)
        return nullptr;
    if (l == r) {
        auto node = newNode(arr[l]);
        update(node);
        return node;
    }
    int m = (l + r) >> 1;
    auto node = newNode(arr[m]);
    node->left = build(arr, l, m - 1);
    node->right = build(arr, m + 1, r);
    update(node);
    return node;
}

PtrTreapNode FPTreap::newNode(int value) {
    auto node = std::make_shared<TreapNode>();
    node->value = value;
    return node;
}

PtrTreapNode FPTreap::newNode(const PtrTreapNode &node) {
    auto _node = std::make_shared<TreapNode>();
    _node->value = node->value;
    _node->sum = node->sum;
    _node->size = node->size;
    _node->left = node->left;
    _node->right = node->right;
    return _node;
}

void FPTreap::print(int l, int r) const {
    print_subtree(last_root.get(), l + 1, r + 1);
    cout << "\n";
}

void FPTreap::print_subtree(TreapNode *node, int l, int r) const {
    if (!node) return;
    int szl = sizeTree(node->left);
    if (szl >= l) {
        print_subtree(node->left.get(), l, r);
    }
    if (l <= szl + 1 && szl + 1 <= r) {
            cout << node->value << " ";
    }
    if (szl + 1 < r) {
        print_subtree(node->right.get(), l - szl - 1, r - szl - 1);
    }
}

void FPTreap::print_subtree(TreapNode *node) const {
    if (!node) return;
    print_subtree(node->left.get());
    cout << node->value << ":" << node->size << std::endl;
    print_subtree(node->right.get());
}

PtrTreapNode FPTreap::split(const PtrTreapNode &root, int size, PtrTreapNode &_left, PtrTreapNode &_right) {
    if (!root) {
        _left = _right = nullptr;
        return nullptr;
    }
    PtrTreapNode node = newNode(root);
    if (sizeTree(node->left) + 1 <= size) {
        split(node->right, size - sizeTree(node->left) - 1, node->right, _right);
        _left = node;

    } else {
        split(node->left, size, _left, node->left);
        _right = node;
    }
    update(_left);
    update(_right);
    return node;
}

PtrTreapNode FPTreap::merge(PtrTreapNode _left, PtrTreapNode _right) {
    if (!_left || !_right) {
        PtrTreapNode node = newNode(_left == nullptr ? _right : _left);
        return node;
    }
    int l = sizeTree(_left);
    int r = sizeTree(_right);
    int rnd = rand() % (l + r);
    if (rnd > r) {
        PtrTreapNode node = newNode(_left);
        node->right = merge(_left->right, _right);
        update(node);
        return node;
    } else {
        PtrTreapNode node = newNode(_right);
        node->left = merge(_left, _right->left);
        update(node);
        return node;
    }
}

// вычисление суммы через перестроение дерева.
unsigned long long FPTreap::sum(int l, int r) {
    PtrTreapNode left, center, right;
    PtrTreapNode old_root = last_root; // можно не мержить потом дерево обратно, а взять старую версию
    split(last_root, l, left, center);
    split(center, r - l + 1, center, right);
    unsigned long long result = center->sum;
    last_root = old_root;
    return result;

//    return sum_subtree(last(),l+1,r+1);
}

// копирование через 4 разреза
void FPTreap::copy(int a, int b, int l) {
    PtrTreapNode left1, center1, right1;
    split(last_root, a, left1, center1);
    split(center1, l, center1, right1);

    PtrTreapNode left2, center2, right2;
    split(last_root, b, left2, center2);
    split(center2, l, center2, right2);

    PtrTreapNode root = merge(left2, center1);
    root = merge(root, right2);

    last_root = root;
}

// вариант с подсчётом обходом, но с умными указателями вместе - долго.
unsigned long long FPTreap::sum_subtree(const PtrTreapNode &node, int l, int r) const {
    if (!node) return 0;
    if (l == 1 && r > node->size)
        return node->sum;
    unsigned long long result = 0;
    int szl = sizeTree(node->left);
    if (szl >= l) {
        result += sum_subtree(node->left, l, r);
    }
    if (l <= szl + 1 && szl + 1 <= r) {
        result += node->value;
    }
    if (szl + 1 < r) {
        result += sum_subtree(node->right, l - szl - 1, r - szl - 1);
    }
    return result;
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);

    int n;
    cin >> n;
    unsigned long long a, b, m;
    int xi;
    cin >> xi >> a >> b >> m;
    int *arr = new int[n];
    arr[0] = xi;
    for (int i = 1; i < n; ++i) {
        xi = static_cast<int>((a * xi + b) % m);
        arr[i] = xi;
    }
    FPTreap tree(arr, n);
    delete[] arr;
    int k;
    cin >> k;
    for (int i = 0; i < k; ++i) {
        string command;
        cin >> command;
        if (command == "cpy") {
            int a, b, l;
            cin >> a >> b >> l;
            tree.copy(a - 1, b - 1, l);
        } else {
            int l, r;
            cin >> l >> r;
            if (command == "sum") {
                cout << tree.sum(l - 1, r - 1) << "\n";
            } else {
                tree.print(l - 1, r - 1);
            }
        }
    }
    return 0;
}
