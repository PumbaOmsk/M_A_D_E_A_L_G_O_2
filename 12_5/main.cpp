/**
 * Введенский Вячеслав В. MADE-ML-22
 *
E. Суффиксный массив + lcp
 */

#include <iostream>
#include <unordered_map>
#include <list>
#include <map>
#include <queue>
#include <stack>
#include <cstring>
#include <algorithm>

static const int MAX_LENGTH = 400000;
static const int BUF_SIZE = 100000;

using namespace std;

/// структура для хранения дерева
struct Node {
    Node() = default;

    inline int len() { return r - l; }

    int l = 0, r = 0, depth = 0;
    bool visited = false;
    Node *parent = nullptr; // родитель
    Node *link = nullptr;   // суффиксная ссылка
    std::map<char, Node *> childs;

    // указатель на вершину по букве
    inline Node *get(char c) {
        if (!childs.count(c)) return nullptr;
        return childs[c];
    }

    // добавить вершину по букве
    inline void set(char c, Node *node) {
        childs[c] = node;
    }

    void computeDepth() {
        depth = r - l;
        if (parent != nullptr)
            depth += parent->depth;
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

    pair<vector<int>, vector<int>> computeSuffixArray();

    // добавить строку к текущей
    void add(const string &s);

private:
    // строка, по которой построено дерево
    string _base;
    const char *base = nullptr; // указатель на кишки строки
    size_t b_len = 0; // длина строки

    // наибольший суффик, который не в терминальной вершине
    Position cursor;
    // заранее выделенная память
    list<Node *> mem_bufs;
    Node *mem_buf; // текущий буфер
    int i_buf = 0; // откуда брать текущий элемент

    Node *root;

    // обработать следующий символ
    void nextChar(int idx);

    // найти позицию для перехода
    Position findPosition(Position from, int l, int r);

    // разрезать ребро по позиции, возвращает указатель на разрез
    Node *splitEdge(Position pos);

    // найти куда должна вести суфф ссылка
    Node *computeLink(Node *v);

    // найти где-нибудь память под вершину
    Node *createNode(int l, int r, Node *parent);
};

SuffixTree::~SuffixTree() {
    for (auto buf:mem_bufs) {
        delete[]buf;
    }
}

Node *SuffixTree::createNode(int l, int r, Node *parent) {
    if (i_buf >= BUF_SIZE) {
        i_buf = 0;
        mem_buf = new Node[BUF_SIZE];
        mem_bufs.push_back(mem_buf);
    }
    Node *newNode = &mem_buf[i_buf++];
    newNode->l = l;
    newNode->r = r;
    newNode->parent = parent;
    newNode->computeDepth();
    return newNode;
}

SuffixTree::SuffixTree() {
    i_buf = 0;
    mem_buf = new Node[BUF_SIZE];
    mem_bufs.push_back(mem_buf);
    root = createNode(0, 0, nullptr);
    cursor = Position(root, 0);
}

Position SuffixTree::findPosition(Position from, int l, int r) {
    Node *v = from.v;
    int fdl = v->len();
    int f_idx = from.idx;
    while (l < r) {
        if (f_idx == fdl) {
            from = Position(v->get(base[l]), 0);
            if (from.v == nullptr)
                return from;
            v = from.v;
            f_idx = from.idx;
            fdl = v->len();
        } else {
            if (base[v->l + f_idx] != base[l])
                return Position(nullptr, 0);
            auto dl = r - l;
            if (dl < fdl - f_idx)
                return Position(v, f_idx + dl);
            l += fdl - f_idx;
            from.idx = fdl;
            f_idx = from.idx;
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
    Node *newNode = createNode(cur->l, cur->l + pos.idx, cur->parent);
    cur->parent->set(base[cur->l], newNode);
    newNode->set(base[cur->l + pos.idx], pos.v);
    pos.v->parent = newNode;
    pos.v->l += pos.idx;
    return newNode;
}

Node *SuffixTree::computeLink(Node *v) {
    if (v->link != nullptr) return v->link;
    if (v->parent == nullptr) return root;
    Node *to = computeLink(v->parent);
    auto end_pos = Position(to, to->len());
    int add = 0;
    if (v->parent->parent == nullptr)
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
        Node *node = createNode(idx, b_len, splitNode);
        splitNode->set(base[idx], node);
        cursor.v = computeLink(splitNode);
        cursor.idx = cursor.v->len();
        if (splitNode == root) break;
    }
}

void SuffixTree::add(const string &s) {
    int old_len = static_cast<int>(_base.length());
    _base = _base + s;
    base = _base.c_str();
    b_len = _base.length();
    for (int i = old_len; i < b_len; ++i) {
        nextChar(i);
    }
}

pair<vector<int>, vector<int>> SuffixTree::computeSuffixArray() {
    stack<Node *> s;
    s.push(root);
    vector<int> suf;
    vector<int> lcp;
    int minDepth = 0;
    while (!s.empty()) {
        auto v = s.top();
        if (v->visited) {
            s.pop();
            if (v->parent != nullptr)
                minDepth = v->parent->depth;
            else minDepth = 0;
            continue;
        }
        if (v->childs.empty()) {
            suf.push_back(b_len - v->depth + 1);
            lcp.push_back(minDepth);
            if (v->parent != nullptr)
                minDepth = v->parent->depth;
            else minDepth = 0;
            s.pop();
        } else {
            v->visited = true;
            if (v->depth < minDepth)
                minDepth = v->depth;
            for (auto it = v->childs.rbegin(); it != v->childs.rend(); it++) {
                s.push(it->second);
            }
        }
    }
    return make_pair(suf, lcp);
}

void print(const vector<int> &v, int start) {
    for (int i = start; i < v.size(); ++i) {
        cout << v[i] << " ";
    }
    cout << endl;
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    string s;
    cin >> s;
    SuffixTree tree;
    tree.add(s + "$");
    auto pair = tree.computeSuffixArray();
    print(pair.first, 1);
    print(pair.second, 2);
    return 0;
}
