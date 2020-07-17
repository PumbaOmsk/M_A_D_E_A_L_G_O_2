/**
 * Введенский Вячеслав В. MADE-ML-22
 B. Задача о назначениях
 */

#include <iostream>
#include <vector>
#include <cassert>
#include <queue>

using namespace std;
/// Некоторое максимальное значение.
static const long long INF = 9223372036854775807;

/// Для хранения информации о ребрах графа
struct EdgeInfo {
    explicit EdgeInfo(int _from, size_t _to, long _cap, long _weight, int _idx) : from(_from), to(_to),
                                                                                  cap(_cap), weight(_weight), flow(0),
                                                                                  idx(_idx) {}

    int from = -1; // откуда идет ребро
    size_t to = 0; // куда ведет ребро
    long cap = 0; // пропускная способность ребра
    long weight = 0; // стоимость единицы потока по ребру
    long flow = 0; // текущий поток по ребру
    EdgeInfo *back = nullptr; // указатель на обратную вершину

    int idx = -1;// номер ребра во входных данных
    // является ли ребро ненасыщенным
    bool nonReplete() {
        return restFlow() > 0;
    }

    // остаточный поток по ребру.
    long restFlow() {
        return cap - flow;
    }
};
//----------------------------------------------------------------------------------------------------------------------
/**
 * ориентированный граф.
 */
class OGraph {
public:
    explicit OGraph(size_t n);

    ~OGraph();

    OGraph(const OGraph &) = delete;

    OGraph &operator=(const OGraph &) = delete;

    OGraph &operator=(const OGraph &&) = delete;

    OGraph(const OGraph &&) = delete;

    void addOr(size_t from, size_t to, long cap, long weight, int idx);

    size_t size() const { return size_; };

    const vector<EdgeInfo *> &edges(size_t node) const;

    long flow(int from) const;

    EdgeInfo *edge(int from, int to, int idx);

private:
    // размер графа - количество вершин
    size_t size_ = 0;
    // список смежных вершин
    vector<EdgeInfo *> *nodes = nullptr;
};

OGraph::OGraph(size_t n) {
    assert(n > 0);
    size_ = n;
    nodes = new vector<EdgeInfo *>[size_];
}

OGraph::~OGraph() {
    for (int i = 0; i < size_; ++i) {
        for (EdgeInfo *edge:nodes[i]) {
            delete edge;
        }
    }
    delete[] nodes;
}

// добавить направленное ребро
void OGraph::addOr(size_t from, size_t to, long cap, long weight, int idx) {
    assert(from < size_ && to < size_);

    auto *forward = new EdgeInfo(from, to, cap, weight, idx);
    auto *backward = new EdgeInfo(to, from, 0, -weight, idx);
    forward->back = backward;
    backward->back = forward;
    nodes[from].emplace_back(forward);
    nodes[to].emplace_back(backward);
}


// список ребер исходящих из вершины
const vector<EdgeInfo *> &OGraph::edges(size_t node) const {
    assert(node < size_);
    return nodes[node];
}

EdgeInfo *OGraph::edge(int from, int to, int idx) {
    for (EdgeInfo *e:nodes[from]) {
        if (e->to == to && e->idx == idx)
            return e;
    }
    return nullptr;
}

// поток из вершины from
long OGraph::flow(int from) const {
    long f = 0;
    for (auto e:nodes[from]) {
        f += e->flow;
    }
    return f;
}

/// Для хранения информации об обходе графа.
struct NodeInfo {
    long long w = INF;
    // откуда зашли в эту вершину.
    EdgeInfo *edge = nullptr;
    bool queue = false;
};

/// Поиск пути минимальной стоимости Форд-Беллман
void findMinCostPathFB(const OGraph &graph, size_t from, size_t to, vector<EdgeInfo *> &path, NodeInfo *nodes) {
    path.clear();
    // идем в ширину начиная с from
    auto sz = graph.size();
    nodes[from].w = 0;
    // очередь для вершин для рассмотрения

    size_t q[sz];
    int head = 0, tail = 0;
    q[tail++] = from;
    if (tail == sz)
        tail = 0;

    nodes[from].queue = true;
    while (tail != head) {
        size_t current = q[head++];
        if (head == sz)
            head = 0;
        nodes[current].queue = false;

        auto neighbour = graph.edges(current);
        auto &node = nodes[current];
        // переберем все ребра из текущей вершины
        for (EdgeInfo *edge:neighbour) {
            //если ребро не насыщенное
            if (edge->cap - edge->flow > 0) {
                auto &nodeTo = nodes[edge->to];
                if (node.w + edge->weight < nodeTo.w) {
                    {
                        nodeTo.w = node.w + edge->weight;
                        nodeTo.edge = edge;

                        if (!nodeTo.queue) {
                            q[tail++] = edge->to;
                            if (tail == sz)
                                tail = 0;
                            nodeTo.queue = true;
                        }
                    }
                }

            }
        }
    }
    int current = static_cast<int>(to);
    while (nodes[current].edge != nullptr) {
        path.push_back(nodes[current].edge);
        current = nodes[current].edge->from;
    }
}

/// Поиск пути минимальной стоимости Дейкстрой
void findMinCostPathD(const OGraph &graph, size_t from, size_t to, vector<EdgeInfo *> &path, NodeInfo *fi) {
    path.clear();
    auto sz = graph.size();
    NodeInfo nodes[sz];
    nodes[from].w = 0;
    // очередь для вершин для рассмотрения
    auto Compare = [](pair<long long, size_t> a, pair<long long, size_t> b) { return a.first > b.first; };
    priority_queue<pair<long long, size_t>, vector<pair<long long, size_t>>, decltype(Compare)> q(Compare);
    q.push(make_pair(nodes[from].w, from));
    nodes[from].queue = true;
    while (!q.empty()) {
        size_t current = q.top().second;
        nodes[current].queue = false;
        q.pop();
        auto neighbour = graph.edges(current);
        auto &node = nodes[current];
        // переберем все ребра из текущей вершины
        for (EdgeInfo *edge:neighbour) {
            //если ребро не насыщенное
            if (edge->cap - edge->flow > 0) {
                auto &nodeTo = nodes[edge->to];
                long long ew = edge->weight + fi[edge->from].w - fi[edge->to].w;
                if (node.w + ew < nodeTo.w) {
                    nodeTo.w = node.w + ew;
                    nodeTo.edge = edge;

                    if (!nodeTo.queue) {
                        q.push(make_pair(nodeTo.w, edge->to));
                        nodeTo.queue = true;
                    }
                }
            }
        }
    }
    for (int i = 0; i < sz; ++i) {
        fi[i].w += nodes[i].w;
    }
    int current = static_cast<int>(to);
    while (nodes[current].edge != nullptr) {
        path.push_back(nodes[current].edge);
        current = nodes[current].edge->from;
    }
}


/**
 * Посчитать в графе максимальный поток.
 * @param g граф.
 */
long long computeMaxFlow(OGraph &g, size_t from, size_t to, vector<EdgeInfo *> &path) {
    auto sz = g.size();
    NodeInfo fi[sz];

    findMinCostPathFB(g, from, to, path, fi);

    long long weight = 0;
    while (!path.empty()) {
        long long mn = INF;
        long long w = 0;

        for (auto e:path) {
            auto mf = e->restFlow();
            w += e->weight;
            if (mf < mn)
                mn = mf;
        }
        for (auto e:path) {
            e->flow += mn;
            e->back->flow -= mn;
        }
        weight += w * mn;

        findMinCostPathD(g, from, to, path, fi);
    }
    return weight;
}

int main() {
    int n;
    cin >> n;
    OGraph g(static_cast<size_t>(2 * n + 2));
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            int w;
            cin >> w;
            g.addOr(i, n + j, 1, w, i * n + j);
        }
        g.addOr(0, i, 1, 0, n * n + i);
        g.addOr(n + i, 2 * n + 1, 1, 0, n * n + n + i);
    }

    int from = 0;
    int to = 2 * n + 1;
    vector<EdgeInfo *> path;
    auto flow = computeMaxFlow(g, from, to, path);
    cout << flow << endl;
    for (int i = 1; i <= n; ++i) {
        auto neighbour = g.edges(i);
        for (EdgeInfo *edge:neighbour) {
            if (edge->flow == 1) {
                cout << i << " " << edge->to - n << endl;
                break;
            }
        }
    }
    return 0;
}
