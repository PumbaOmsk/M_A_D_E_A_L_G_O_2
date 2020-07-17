/**
 * Введенский Вячеслав В. MADE-ML-22
 * A. Персистентный стек
 * версия, которую можно модифицировать
 */
#include <iostream>
#include <vector>

using namespace std;

struct Node {
    Node() : prev(nullptr), v(0) {}

    Node(Node *_prev, int _v) : prev(_prev), v(_v) {}

    Node *prev;
    int v;
};

struct FPStack {
    FPStack() {
        Node *p = new Node(nullptr, 0);
        elements.push_back(p);
        versions.push_back(p);
    }

    ~FPStack() {
        for (auto &element : elements) {
            delete element;
        }
    }

    vector<Node *> versions; // версии стека
    vector<Node *> elements; // храним элементы, чтобы знать какие удалять.

    void push(int idx, int v) {
        Node *p = new Node(versions[idx], v);
        elements.push_back(p);
        versions.push_back(p);
    }

    int pop(int idx) {
        versions.push_back(versions[idx]->prev);
        return versions[idx]->v;
    }
};


int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);

    FPStack st;
    int n;
    cin >> n;
    for (int i = 0; i < n; ++i) {
        int t, m;
        cin >> t >> m;
        if (m > 0) {
            st.push(t, m);
        } else {
            cout << st.pop(t) << "\n";
        }
    }
    return 0;
}