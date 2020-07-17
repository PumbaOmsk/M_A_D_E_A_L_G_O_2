/**
 * Введенский Вячеслав В. MADE-ML-22
 *
C. Множественный поиск 2.
 Дан массив строк s i и строка t. Требуется для каждой строки s i определить, сколько раз она встречается в t как подстрока.
 */

#include <iostream>
#include <unordered_map>
#include <list>
#include <map>
#include <queue>
#include <stack>
#include <cstring>

static const int MAX_LENGTH = 1000000;

using namespace std;

/// структура для хранения дерева
struct Node {
    int l = 0, r = 0, dl = 0; // индексация ребра в строке
    Node *parent = nullptr; // родитель
    Node *link = nullptr;   // суффиксная ссылка
    Node *childs[27] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
                        nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
//    std::unordered_map<char, Node *> childs; // детишки, для букв. Можно сюда массив запихать
    int index = -1; // номер вершины

    long long leafCount = -1;

    // указатель на вершину по букве
    inline Node *get(char c) {
//        if (!childs.count(c)) return nullptr;
//        return childs[c];
//        if(c=='$')
//            return childs[26];
        return childs[c - 'a'];
    }

    bool leaf = true;

    // добавить вершину по букве
    inline void set(char c, Node *node) {
        childs[c - 'a'] = node;
        leaf = false;

//        if(c=='$')
//            childs[26] = node;
//        else {
//            childs[c - 'a'] = node;
//        }
//        childs[c] = node;
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

    SuffixTree(int n);

    void print();

    // финальная печать ребер дерева
    void printEdges();

    // количество различных подстрок
    long long computeSubString();

    // посчитать количество листов под каждой вершиной.
    void computeLeaf();

    // поиск подстроки
    Node *find(const string &s);

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
    Node *mem_buf;
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

    Node *createNode(int l, int r, Node *parent);
};

SuffixTree::~SuffixTree() {
    delete[] mem_buf;
}

Node *SuffixTree::createNode(int l, int r, Node *parent) {
    Node *newNode = &mem_buf[currentIndex];
    currentIndex++;
    newNode->l = l;
    newNode->r = r;
    newNode->dl = r - l;
    newNode->parent = parent;
    newNode->leafCount = -1;
    newNode->leaf = true;
    return newNode;
}

SuffixTree::SuffixTree(int n) {
    currentIndex = 0;
    mem_buf = new Node[n];
//    memset(mem_buf,0,n* sizeof(Node));
    root = createNode(0, 0, nullptr);
//    root = new Node(++currentIndex, 0, 0, nullptr);
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
            if (child != nullptr)
                q.push_back(child);
//            q.push_back(child.second);
        }
    }
//    q.sort(comp);
    cout << q.size() << " " << q.size() - 1 << endl;
    for (auto cur:q) {
//        if (cur->isRoot())
        if (cur->parent == nullptr)
            continue;
        cout << cur->parent->index << " " << cur->index << " " << cur->l + 1 << " " << cur->r << endl;
    }
}

//void SuffixTree::print(Node *pNode) {
//    if (pNode == nullptr)
//        return;
//    if (pNode->isRoot()) {
//        cout << pNode->index << ":" << "root";
//    } else {
//        cout << pNode->parent->index << "->" << pNode->index;
//    }
//    cout << ":" << base.substr(pNode->l, pNode->len()) << "[" << pNode->l << "," << pNode->r << "):";
//    for (auto child:pNode->childs) {
//        cout << child.first << " ";
//    }
//    cout << endl;
//
//    for (auto child:pNode->childs) {
//        cout << child.first << " ";
//        print(child.second);
//    }
//}

//void SuffixTree::print() {
//    print(root);
//}

Position SuffixTree::findPosition(Position from, int l, int r) {
    while (l < r) {
//        if (from.idx == from.v->len()) {
        if (from.idx == from.v->dl) {
            from = Position(from.v->get(base[l]), 0);
            if (from.v == nullptr)
                return from;
        } else {
            if (base[from.v->l + from.idx] != base[l])
                return Position(nullptr, 0);
            auto dl = r - l;
            auto fdl = from.v->dl;
            if (dl < fdl - from.idx)
                return Position(from.v, from.idx + dl);
            l += fdl - from.idx;
            from.idx = fdl;
        }
    }
    return from;
}

Node *SuffixTree::splitEdge(Position pos) {
    if (pos.idx == pos.v->dl)
        return pos.v;
    if (pos.idx == 0)
        return pos.v->parent;
    Node *cur = pos.v;
    Node *newNode = createNode(cur->l, cur->l + pos.idx, cur->parent);
    cur->parent->set(base[cur->l], newNode);
    newNode->set(base[cur->l + pos.idx], pos.v);
    pos.v->parent = newNode;
    pos.v->l += pos.idx;
    pos.v->dl = pos.v->r - pos.v->l;
    return newNode;
}

Node *SuffixTree::computeLink(Node *v) {
    if (v->link != nullptr) return v->link;
    if (v->parent == nullptr) return root;
    Node *to = computeLink(v->parent);
    auto end_pos = Position(to, to->dl);
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
        splitNode->set(base[idx], createNode(idx, base.length(), splitNode));
        cursor.v = computeLink(splitNode);
        cursor.idx = cursor.v->dl;
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
        if (cur->parent != nullptr && cur->leaf) {
            cur->r = static_cast<int>(len);
            cur->dl = cur->r - cur->l;
        } else {
            for (auto child:cur->childs) {
                if (child != nullptr)
                    q.push(child);
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

long long SuffixTree::computeSubString() {
    queue<Node *> q;
    q.push(root);
    long long result = 0;
    while (!q.empty()) {
        auto cur = q.front();
        q.pop();
        for (auto child:cur->childs) {
            if (child != nullptr)
                q.push(child);
//            q.push(child.second);
        }
        if (cur->parent != nullptr) {
            result += cur->dl;
//            result += cur->len();
        }
    }
    return result;
}

void SuffixTree::computeLeaf() {
    int cur = -1;
    Node **s = new Node *[currentIndex];
    s[++cur] = root;
    while (cur >= 0) {
        auto v = s[cur];
        if (v->leafCount > -1) {
            cur--;
            if (v->parent != nullptr)
                v->parent->leafCount += v->leafCount;
        } else {
            if (v->leaf) {
                v->leafCount = 1;
            } else {
                v->leafCount = 0;
                for (auto child:v->childs) {
                    if (child != nullptr)
                        s[++cur] = child;
                }
            }
        }
    }
    delete[]s;
}

Node *SuffixTree::find(const string &s) {
    Node *cur = root;
    int idx = 0;
    int i = 0;
    while (i < s.length()) {
        char c = s[i];
        while (idx < cur->dl) {
            if (base[cur->l + idx] != c) {
                return nullptr;
            }
            idx++;
            ++i;
            if (i >= s.length())
                return cur;
            c = s[i];
        }
        auto child = cur->get(c);
        if (child == nullptr) {
            return nullptr;
        } else {
            cur = child;
            idx = 0;
        }
    }
    return cur;
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);

    int n;
    cin >> n;
    vector<string> lines;
    for (int i = 0; i < n; ++i) {
        string s;
        cin >> s;
        lines.push_back(s);
    }
    string t;
    cin >> t;
    SuffixTree tree(2 * MAX_LENGTH);
    tree.add(t + "{");

    tree.computeLeaf();
    for (int i = 0; i < n; ++i) {
        Node *v = tree.find(lines[i]);
        if (v == nullptr)
            cout << "0" << "\n";
        else
            cout << v->leafCount << "\n";
    }

    cout << endl;
    return 0;
}
