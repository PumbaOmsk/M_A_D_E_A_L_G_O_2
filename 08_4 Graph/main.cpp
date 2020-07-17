/**
 * Введенский Вячеслав В. MADE-ML-22
 * D. Великая стена
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
/// Максимальный поток через болота.
static const int FLOW_SWAMP = 2500;
/// Максимальный поток через город.
static const int FLOW_CITY = 5 * 2500;
/// Максимальный поток через равнину.
static const int FLOW_PLAIN = 1;
/// типы ячеек на поле
enum CellType {
    EMPTY = ' ',
    CITY_A = 'A',   // "A" - город А
    CITY_B = 'B',   // "B" - город Б
    MOUNTAIN = '#', // "#" - гора - нет прохода
    SWAMP = '-',    // "-" - болото - нельзя строить
    PLAIN = '.',    // "." - равнина - можно строить
    WALL = '+'      // "+" - стена
};


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
vector<EdgeInfo *> findMinPath(const OGraph &graph, size_t from, size_t to) {
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
    auto path = findMinPath(g, from, to);
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
        path = findMinPath(g, from, to);
    }
}

/**
 * @return множество вершин, которые доступны из from.
 */
set<size_t> computeCut(OGraph &g, size_t from) {
    // идем вширину начиная с from
    auto sz = g.size();
    NodeInfo nodes[sz];
    nodes[from].step = 1;
    // очередь для вершин для рассмотрения
    queue<size_t> q;
    set<size_t> cut;
    q.push(from);
    while (!q.empty()) {
        size_t current = q.front();
        q.pop();
        cut.insert(current);
        auto neighbour = g.edges(current);
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
    return cut;
}

/// печать поля
void print(vector<vector<CellType>> &plan) {
    for (int y = 0; y < plan.size(); ++y) {
        auto row = plan[y];
        for (int x = 0; x < row.size(); ++x) {
            cout << (char) row[x];
        }
        cout << endl;
    }
    cout << endl;
}

/// возвращает тип клетки по индексу (с проверкой на выход за пределы поля)
CellType getCellType(const vector<vector<CellType>> &plan, int x, int y) {
    if (x < 0 || y < 0 || y >= plan.size() || x >= plan[y].size())
        return EMPTY;
    return plan[y][x];
}

/// создает одно ребро
void createEdge(OGraph &g, const vector<vector<CellType>> &plan, size_t from, int x, int y) {
    CellType cell = getCellType(plan, x, y);
    if (cell == EMPTY || cell == CITY_A || cell == MOUNTAIN)
        return;
    size_t to = y * plan[y].size() + x;
    switch (cell) {
        case CITY_B:
            g.addOr(from, to, FLOW_CITY, 0);
            break;
        case SWAMP:
            g.addOr(from, to, FLOW_SWAMP, 0);
            break;
        case PLAIN:
            g.addOr(from, to, FLOW_PLAIN, 0);
            return;
    }
}

/// создает одно внутреннее ребро в ячеёке
void createEdgeInner(OGraph &g, const vector<vector<CellType>> &plan, size_t from, size_t to, CellType cell) {
    if (cell == EMPTY || cell == CITY_A || cell == MOUNTAIN)
        return;
    switch (cell) {
        case CITY_B:
            g.addOr(from, to, FLOW_CITY, 0);
            break;
        case SWAMP:
            g.addOr(from, to, FLOW_SWAMP, 0);
            break;
        case PLAIN:
            g.addOr(from, to, FLOW_PLAIN, 0);
            return;
    }
}

/// заполняет граф ребрами в соответствии с планом
void fillEdges(OGraph &g, const vector<vector<CellType>> &plan) {
    auto m = plan.size();
    for (int y = 0; y < m; ++y) {
        auto n = plan[y].size();
        auto shift = n * m;
        for (int x = 0; x < n; ++x) {
            auto cell = plan[y][x];
            if (cell == MOUNTAIN || cell == CITY_B || cell == EMPTY)
                continue;
            size_t from = y * n + x;
            createEdge(g, plan, from + shift, x, y - 1);
            createEdge(g, plan, from + shift, x, y + 1);
            createEdge(g, plan, from + shift, x - 1, y);
            createEdge(g, plan, from + shift, x + 1, y);
            createEdgeInner(g, plan, from, from + shift, cell);
        }
    }
}

int main() {
    size_t n, m;
    cin >> m >> n;
    vector<vector<CellType>> plan(m, vector<CellType>(n, EMPTY));

    size_t from, to;
    for (int y = 0; y < m; ++y) {
        for (int x = 0; x < n; ++x) {
            char c;
            cin >> c;
            plan[y][x] = static_cast<CellType>(c);
            if (c == CITY_A) {
                from = x + y * n;
            } else if (c == CITY_B) {
                to = x + y * n;
            }
        }
    }

    int shift = m * n;
    OGraph g(2 * n * m);
    fillEdges(g, plan);
    from += shift;
    computeMaxFlow(g, from, to);
    auto flow = g.flow(from);
    if (flow >= FLOW_SWAMP) {
        cout << "-1" << endl;
        return 0;
    }
    if (flow == 0) {
        cout << "0" << endl;
        print(plan);
        return 0;
    }

    auto cut = computeCut(g, from);
    for (int i = 0; i < 2 * n * m; ++i) {
        if (cut.count(i) == 0)
            continue;
        auto edges = g.edges(i);
        for (EdgeInfo *e:edges) {
            auto _to = e->to;
            if (e->cap == 0 || cut.count(_to) > 0) //e->cap==0 ||
                continue;
            if (_to >= shift) {
                _to -= shift;
            }
            int x = _to % n;
            int y = (_to - x) / n;
            plan[y][x] = WALL;
        }
    }
    cout << flow << endl;
    print(plan);

    return 0;
}
