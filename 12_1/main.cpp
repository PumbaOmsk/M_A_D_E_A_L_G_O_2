/**
 * Введенский Вячеслав В. MADE-ML-22
 *
A. Суффиксное дерево - Постройте суффиксное дерево для заданной строки s.
 */

#include <iostream>
#include <unordered_map>
#include <list>
#include <map>
#include <queue>

using namespace std;

/// структура для хранения дерева
struct Node {
    Node(int _index, int _l, int _r, Node *_parent) : index(_index), l(_l), r(_r), parent(_parent), link(nullptr) {}

    ~Node() {
        for (auto child:childs) {
            delete child.second;
        }
    }

    // длина ребра в символах
    inline int len() { return r - l; }

    int l, r; // индексация ребра в строке
    Node *parent; // родитель
    Node *link;   // суффиксная ссылка
    std::unordered_map<char, Node *> childs; // детишки, для букв. Можно сюда массив запихать
    int index; // номер вершины

    // указатель на вершину по букве
    inline Node *get(char c) {
        if (!childs.count(c)) return nullptr;
        return childs[c];
    }

    // добавить вершину по букве
    inline void set(char c, Node *node) {
        childs[c] = node;
    }

    // является ли вершина корнем
    inline bool isRoot() {
        return parent == nullptr;
    }
};

/// положение в дереве
struct Position {
    explicit Position(Node *_v, int _idx) : v(_v), idx(_idx) {}

    Position() = default;

    Node *v = nullptr; // текущая вершина
    int idx = 0; // сдвиг в подстроке
};

/// суффиксное дерево
class SuffixTree {
public:
    ~SuffixTree();

    SuffixTree();

    void print();

    // финальная печать ребер дерева
    void printEdges();

    // добавить строку к текущей
    void add(const string &s);


private:
    // строка, по которой построено дерево
    string base;
    // текущий номер вершины
    int currentIndex;
    // наибольший суффик, который не в терминальной вершине
    Position cursor;
    // если будет тупить, то можно заранее выделить память массив и просто дописывать туда по номеру вершины
    Node *root;

    void print(Node *pNode);

    // обработать следующий символ
    void nextChar(int idx);

    // найти позицию для перехода
    Position findPosition(Position from, int l, int r);

    // разрезать ребро по позиции, возвращает указатель на разрез
    Node *splitEdge(Position pos);

    // найти куда должна вести суфф ссылка
    Node *computeLink(Node *v);

    // обновить листики
    void updateLeaf();
};

SuffixTree::~SuffixTree() {
    delete root;
}

SuffixTree::SuffixTree() {
    currentIndex = 0;
    root = new Node(++currentIndex, 0, 0, nullptr);
    cursor = Position(root, 0);
}

// для сортировки ребер, если надо
//bool comp(Node *e1, Node *e2) {
//    return e1->l < e2->l;
//}

void SuffixTree::printEdges() {
    list<Node *> q;
    q.push_back(root);
    for (auto it = q.begin(); it != q.end(); it++) {
        auto cur = *it;
        for (auto child:cur->childs) {
            q.push_back(child.second);
        }
    }
//    q.sort(comp);
    cout << q.size() << " " << q.size() - 1 << endl;
    for (auto cur:q) {
        if (cur->isRoot())
            continue;
        cout << cur->parent->index << " " << cur->index << " " << cur->l + 1 << " " << cur->r << endl;
    }
}

void SuffixTree::print(Node *pNode) {
    if (pNode == nullptr)
        return;
    if (pNode->isRoot()) {
        cout << pNode->index << ":" << "root";
    } else {
        cout << pNode->parent->index << "->" << pNode->index;
    }
    cout << ":" << base.substr(pNode->l, pNode->len()) << "[" << pNode->l << "," << pNode->r << "):";
    for (auto child:pNode->childs) {
        cout << child.first << " ";
    }
    cout << endl;

    for (auto child:pNode->childs) {
        cout << child.first << " ";
        print(child.second);
    }
}

void SuffixTree::print() {
    print(root);
}

Position SuffixTree::findPosition(Position from, int l, int r) {
    while (l < r) {
        if (from.idx == from.v->len()) {
            from = Position(from.v->get(base[l]), 0);
            if (from.v == nullptr)
                return from;
        } else {
            if (base[from.v->l + from.idx] != base[l])
                return Position(nullptr, 0);
            auto dl = r - l;
            if (dl < from.v->len() - from.idx)
                return Position(from.v, from.idx + dl);
            l += from.v->len() - from.idx;
            from.idx = from.v->len();
        }
    }
    return from;
}

Node *SuffixTree::splitEdge(Position pos) {
    if (pos.idx == pos.v->len())
        return pos.v;
    if (pos.idx == 0)
        return pos.v->parent;
    Node *cur = pos.v;
    Node *newNode = new Node(++currentIndex, cur->l, cur->l + pos.idx, cur->parent);
    cur->parent->set(base[cur->l], newNode);
    newNode->set(base[cur->l + pos.idx], pos.v);
    pos.v->parent = newNode;
    pos.v->l += pos.idx;
    return newNode;
}

Node *SuffixTree::computeLink(Node *v) {
    if (v->link != nullptr) return v->link;
    if (v->isRoot()) return root;
    Node *to = computeLink(v->parent);
    auto end_pos = Position(to, to->len());
    int add = 0;
    if (v->parent->isRoot())
        add = 1;
    auto target = findPosition(end_pos, v->l + add, v->r);
    v->link = splitEdge(target);
    return v->link;
}

void SuffixTree::nextChar(int idx) {
    while (true) {
        Position fnd = findPosition(cursor, idx, idx + 1);
        if (fnd.v != nullptr) {
            cursor = fnd;
            return;
        }
        Node *splitNode = splitEdge(cursor);
        splitNode->set(base[idx], new Node(++currentIndex, idx, base.length(), splitNode));
        cursor.v = computeLink(splitNode);
        cursor.idx = cursor.v->len();
        if (splitNode == root) break;
    }
}

void SuffixTree::updateLeaf() {
    auto len = base.length();
    queue<Node *> q;
    q.push(root);
    while (!q.empty()) {
        auto cur = q.front();
        q.pop();
        if (!cur->isRoot() && cur->childs.empty()) {
            cur->r = static_cast<int>(len);
        } else {
            for (auto child:cur->childs) {
                q.push(child.second);
            }
        }
    }

}

void SuffixTree::add(const string &s) {
    int old_len = static_cast<int>(base.length());
    base = base + s;
    updateLeaf();
    for (int i = old_len; i < base.length(); ++i) {
//        cout << i << "-----------------------------" << endl;
        nextChar(i);
//        print();
    }
}

int main() {
    string s;// = "ababb";
    cin>>s;
    SuffixTree tree;
    tree.add(s);
//    tree.print();
    tree.printEdges();
    return 0;
}

