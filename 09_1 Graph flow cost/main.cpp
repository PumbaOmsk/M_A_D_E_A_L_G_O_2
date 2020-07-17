/**
 * Введенский Вячеслав В. MADE-ML-22
A. Максимальный поток минимальной стоимости
 */
#include <iostream>
#include <vector>
#include <cassert>
#include <queue>

using namespace std;
/**
 * Некоторое максимальное значение.
 */
static const long long INF = 9223372036854775807;

/**
 * Для хранения информации о ребрах графа
 */
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

/**
 * Для хранения информации об обходе графа.
 */
struct NodeInfo {
    // номер шага на котором зашли в эту вершину.
    int step = 0;
    long long w = INF;
    // откуда зашли в эту вершину.
    EdgeInfo *edge = nullptr;
    bool queue = false;

    // обновить информацию о вершине по предыдущей
    bool update(const NodeInfo &prevNode, EdgeInfo *_edge);
};

/**
 * обновить информацию о вершине по предыдущей.
 * @param prevNode предыдущая вершина.
 * @param _edge ребро, по которому пришли в эту вершину (для построения обратного пути).
 * @return true, если необходимо добавить вершину в очередь на рассмотрение (то есть первый раз зашли).
 */
bool NodeInfo::update(const NodeInfo &prevNode, EdgeInfo *_edge) {
    // стоимость пути до этой вершины меньше через ребро.
    if (w == INF || prevNode.w + _edge->weight < w) {
        step = prevNode.step + 1;
        w = prevNode.w + _edge->weight;
        edge = _edge;
        return true; // тогда надо добавить в очередь
    }
    return false;
}

vector<EdgeInfo *> findMinCostPath(const OGraph &graph, size_t from, size_t to) {
    // идем в ширину начиная с from
    auto sz = graph.size();
    NodeInfo nodes[sz];
    nodes[from].step = 1;
    nodes[from].w = 0;
    // очередь для вершин для рассмотрения
    queue<size_t> q;
    q.push(from);
    nodes[from].queue = true;
    while (!q.empty()) {
        size_t current = q.front();
        nodes[current].queue = false;
        q.pop();
        auto neighbour = graph.edges(current);
        auto &node = nodes[current];
        // переберем все ребра из текущей вершины
        for (EdgeInfo *edge:neighbour) {
            //если ребро не насыщенное
            if (edge->nonReplete()) {
                if (nodes[edge->to].update(node, edge)) {
                    if (!nodes[edge->to].queue) {
                        q.push(edge->to);
                        nodes[edge->to].queue = true;
                    }
                }
            }
        }
    }
    int current = static_cast<int>(to);
    vector<EdgeInfo *> path;

    while (nodes[current].edge != nullptr) {
        path.push_back(nodes[current].edge);
        current = nodes[current].edge->from;
    }
    return path;
}

/**
 * Посчитать в графе максимальный поток.
 * @param g граф.
 */
long long computeMaxFlow(OGraph &g, size_t from, size_t to) {
    auto path = findMinCostPath(g, from, to);
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
        path = findMinCostPath(g, from, to);
    }
    return weight;
}


int main() {
    int n, m;
    cin >> n >> m;
    OGraph g(static_cast<size_t>(n));
    for (int i = 0; i < m; ++i) {
        int u, v, c, w;
        cin >> u >> v >> c >> w;
        g.addOr(u - 1, v - 1, c, w, i);
    }
    int from = 0;
    int to = n - 1;
    auto flow = computeMaxFlow(g, from, to);
    cout << flow << endl;

    return 0;
}