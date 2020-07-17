/**
 * Введенский Вячеслав, MADE-22
 *
 E. Урны и шары - на умных указателях
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
    explicit TreapNode() : index(-1), size(1), left(nullptr), right(nullptr), upd(0) {}

    int index;                // номер урны
    int upd;                  // сдвиг для номера урны, нужен для обновления при пересыпании
    list<int> balls;          // список мячей в урне
    int size;                 // размер поддерева.
    PtrTreapNode left;
    PtrTreapNode right;
};

// проталкивание обновления индекса ниже
void updDown(PtrTreapNode &node) {
    if (!node)
        return;
    if (node->upd != 0) {
        node->index += node->upd;
        if (node->left)
            node->left->upd += node->upd;
        if (node->right)
            node->right->upd += node->upd;
        node->upd = 0;
    }
}

// значение размера дерева.
int _sizeTree(const PtrTreapNode &node) {
    return node == nullptr ? 0 : node->size;
}

// значение размера дерева.
int indexTree(const PtrTreapNode &node) {
    return node == nullptr ? 0 : node->index;
}

// пересчёт размера дерева, при условии, что дети корректны.
static void update(const PtrTreapNode &node) {
    if (node) {
        node->size = _sizeTree(node->left) + _sizeTree(node->right) + 1;
    }
};

// персистентное дерево, но при этом не храним версии.
class FPTreap {
public:
    FPTreap();

    FPTreap(const int *a, int n);

    void shift(int a, int b, int l);

    void getNodes(vector<PtrTreapNode> &nodes);

private:
    PtrTreapNode last_root = nullptr; // текущий корень

    PtrTreapNode newNode(int value);

    PtrTreapNode newNode(const PtrTreapNode &);

    PtrTreapNode split(PtrTreapNode &node, int index, PtrTreapNode &_left, PtrTreapNode &_right);

    PtrTreapNode merge(PtrTreapNode &_left, PtrTreapNode &_right);

    // построить дерево из массива
    PtrTreapNode build(const int *arr, int l, int r);

    // вернуть вектор всех урн
    void getNodes(PtrTreapNode &node, vector<PtrTreapNode> &nodes);
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

// для первичной инициализации
PtrTreapNode FPTreap::newNode(int index) {
    auto node = std::make_shared<TreapNode>();
    node->index = index;
    node->balls.push_back(index);
    return node;
}

//создание копии
PtrTreapNode FPTreap::newNode(const PtrTreapNode &node) {
    auto _node = std::make_shared<TreapNode>();
    _node->index = node->index;
    _node->balls.splice(_node->balls.end(), node->balls);
    _node->size = node->size;
    _node->left = node->left;
    _node->right = node->right;
    _node->upd = node->upd;
    return _node;
}

void FPTreap::getNodes(PtrTreapNode &node, vector<PtrTreapNode> &nodes) {
    if (!node) return;
    updDown(node);
    getNodes(node->left, nodes);
    nodes.push_back(node);
    getNodes(node->right, nodes);
}

void FPTreap::getNodes(vector<PtrTreapNode> &nodes) {
    getNodes(last_root, nodes);
}


PtrTreapNode FPTreap::split(PtrTreapNode &root, int index, PtrTreapNode &_left, PtrTreapNode &_right) {
    if (!root) {
        _left = _right = nullptr;
        return nullptr;
    }
    updDown(root);
    PtrTreapNode node = newNode(root);
    if (indexTree(node) < index) {
        split(node->right, index, node->right, _right);
        _left = node;

    } else {
        split(node->left, index, _left, node->left);
        _right = node;
    }
    update(_left);
    update(_right);
    return node;
}

PtrTreapNode FPTreap::merge(PtrTreapNode &_left, PtrTreapNode &_right) {
    updDown(_left);
    updDown(_right);
    if (!_left || !_right) {
        PtrTreapNode node;
        if (_left)
            node = newNode(_left);
        else if (_right)
            node = newNode(_right);
        else node = nullptr;
        return node;
    }
    int l = _sizeTree(_left);
    int r = _sizeTree(_right);
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

// переложить мячики
void FPTreap::shift(int a, int b, int l) {
    PtrTreapNode left1, center1, right1;
    // center1 - откуда перекладываем
    split(last_root, a, left1, center1);
    split(center1, a + l, center1, right1);

    // склеим после вырезания
    auto crop = merge(left1, right1);

    PtrTreapNode left2, center2, right2;
    // center2 - куда перекладываем (если есть пересечение, то вырежется)
    split(crop, b, left2, center2);
    split(center2, b + l, center2, right2);

    // будем перекладывать из меньшей в большую
    PtrTreapNode from, to;
    int index_shift = 0;// означает, что надо при поиске добавлять сдвиг номеров урн.
    if (_sizeTree(center1) > _sizeTree(center2)) {
        center1->upd += b - a;// потребуется сдвиг номеров урн, если center1 будет приемников мячей
        to = center1;
        from = center2;
    } else {
        index_shift = b - a;
        from = center1;
        to = center2;
    }

    // надо склеить center1 и center2, делаем это поштучно
    vector<PtrTreapNode> nodes;
    getNodes(from, nodes);// все урны поштучно
    for (auto &node:nodes) {
        PtrTreapNode left, center, right;
        int idx = node->index + index_shift;
        // ищем куда вставить урну.
        split(to, idx, left, center);
        split(center, idx + 1, center, right);
        if (center.get() == nullptr) {
            node->left = nullptr;
            node->right = nullptr;
            node->index = idx;
            to = merge(left, node);
            to = merge(to, right);
        } else {// а если она уже есть - просто пересыпем шары.
            center->balls.splice(center->balls.end(), node->balls);
            to = merge(left, center);
            to = merge(to, right);
        }
    }
    // склеим обратно дерево
    last_root = merge(left2, to);
    last_root = merge(last_root, right2);
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    int n, m;
    cin >> n >> m;
    int *balls = new int[n];
    for (int i = 0; i < n; ++i) {
        balls[i] = i;
    }
    FPTreap tree(balls, n);
    for (int i = 0; i < m; ++i) {
        int l, a, b;
        cin >> l >> a >> b;
        tree.shift(a - 1, b - 1, l);
    }


    vector<PtrTreapNode> nodes;
    tree.getNodes(nodes);
    for (auto &node : nodes) {
        int index = node->index;
        for (auto ball:node->balls) {
            balls[ball] = index;
        }
    }
    for (int i = 0; i < n; ++i) {
        cout << balls[i] + 1 << " ";
    }
    delete[] balls;

    return 0;
}
