/**
 * Введенский Вячеслав В. MADE-ML-22
 *
C. Улиточки
 найти два пути из s в t.
 */
#include <iostream>
#include <vector>
#include <cassert>
#include <queue>
#include <list>
#include <set>

using namespace std;
/**
 * Некоторое максимальное значение.
 */
static const long long INF = static_cast<const long>(1e18);

/**
 * Для хранения информации о ребрах графа
 */
struct EdgeInfo {
    explicit EdgeInfo(int _from, size_t _to, long _cap, int _idx) : from(_from), to(_to), cap(_cap), flow(0),
                                                                    idx(_idx) {}

    int from = -1; // откуда идет ребро
    size_t to = 0; // куда ведет ребро
    long cap = 0; // пропускная способность ребра
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

    void add(size_t from, size_t to, long cap, int idx);

    void addOr(size_t from, size_t to, long cap, int idx);

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

// добавить ненаправленное ребро (обратно тоже есть cap)
void OGraph::add(size_t from, size_t to, long cap, int idx) {
    assert(from < size_ && to < size_);

    auto *forward = new EdgeInfo(from, to, cap, idx);
    auto *backward = new EdgeInfo(to, from, cap, idx);
    forward->back = backward;
    backward->back = forward;
    nodes[from].emplace_back(forward);
    nodes[to].emplace_back(backward);
}

// добавить направленное ребро
void OGraph::addOr(size_t from, size_t to, long cap, int idx) {
    assert(from < size_ && to < size_);

    auto *forward = new EdgeInfo(from, to, cap, idx);
    auto *backward = new EdgeInfo(to, from, 0, idx);
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
    // откуда зашли в эту вершину.
    EdgeInfo *edge = nullptr;

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
    // если в этой вершиние еще не были
    if (step == 0) {
        step = prevNode.step + 1;
        edge = _edge;
        return true; // тогда надо добавить в очередь
    }
    return false;
}

/**
 * @return массив ребер в минимальном пути.
 */
vector<EdgeInfo *> find_min_path(const OGraph &graph, size_t from, size_t to) {
    // идем вширину начиная с from
    auto sz = graph.size();
    NodeInfo nodes[sz];
    nodes[from].step = 1;
    // очередь для вершин для рассмотрения
    queue<size_t> q;
    q.push(from);
    while (!q.empty()) {
        size_t current = q.front();
        if (current == to) {
            break;
        }
        q.pop();
        auto neighbour = graph.edges(current);
        auto &node = nodes[current];
        // переберем все ребра из текущей вершины
        for (EdgeInfo *edge:neighbour) {
            //если ребро не насыщенное
            if (edge->nonReplete()) {
                if (nodes[edge->to].update(node, edge)) {
                    q.push(edge->to);
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
void computeMaxFlow(OGraph &g, size_t from, size_t to) {
    auto path = find_min_path(g, from, to);
    while (!path.empty()) {
        long long mn = INF;
        for (auto e:path) {
            auto mf = e->restFlow();
            if (mf < mn)
                mn = mf;
        }
        for (auto e:path) {
            e->flow += mn;
            e->back->flow -= mn;
        }
        path = find_min_path(g, from, to);
    }
}

/**
 * @return вектор вершин пути улитки. с занулением потока этого пути.
 */
vector<int> findFlowPath(OGraph &g, int from, int to) {
    vector<int> path;
    path.push_back(from);
    int current = from;
    while (current != to) {
        auto neighbour = g.edges(current);
        for (EdgeInfo *edge:neighbour) {
            if (edge->flow == 1) {
                path.push_back(edge->to);
                edge->flow = 0;
                current = static_cast<int>(edge->to);
                break;
            }
        }
    }
    return path;
}

int main() {
    int n, m, s, t;
    cin >> n >> m >> s >> t;
    OGraph g(static_cast<size_t>(n));
    for (int i = 0; i < m; ++i) {
        size_t a, b;
        cin >> a >> b;
        g.addOr(a - 1, b - 1, 1, i);
    }
    int from = s - 1;
    int to = t - 1;
    computeMaxFlow(g, from, to);
    auto flow = g.flow(from);
    if (flow >= 2) {
        cout << "YES" << endl;
        auto path = findFlowPath(g, from, to);
        for (int v:path) {
            cout << v + 1 << " ";
        }
        cout << endl;
        path = findFlowPath(g, from, to);
        for (int v:path) {
            cout << v + 1 << " ";
        }
        cout << endl;
    } else {
        cout << "NO" << endl;
    }
    return 0;
}