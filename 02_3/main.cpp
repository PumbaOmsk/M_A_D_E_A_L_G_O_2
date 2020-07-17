/*
 * Введенский Вячеслав В. MADE-ML-22
 *
 C. Выбор вершин дерева
Дан граф, являющийся деревом. Необходимо найти размер наибольшего по мощности независимого множества.
Множество вершин графа называется независимым, если никакие две вершины этого множества не соединены ребром.

 Входные данные
Граф в этой задаче задан в виде корневого дерева.
 Для каждой вершины i, не являющейся корнем, задан номер вершины-предка pi в корневом дереве.
 Дерево, заданное таким образом, состоит из рёбер i—pi для всех вершин i, кроме корня.

В первой строке входного файла записано целое число n — количество вершин в графе (1 ≤ n ≤ 100). В следующих n строках
задан граф. В i-й из этих строк записано целое число pi — номер вершины-предка i-й вершины.
 Для корня дерева pi = 0; для всех остальных вершин 1 ≤ pi ≤ n.

Выходные данные
В первой строке выходного файла выведите одно число — размер наибольшего по мощности независимого множества..
 */
#include <iostream>
#include <vector>
#include <list>
#include <stack>
#include <queue>

using namespace std;

/**
 * Одна вершина дерева с номерами своих детишек.
 */
struct TreeNode {
    list<int> child; // список детей вершины.

    void add(int c) {
        child.push_back(c);
    }
};

/**
 * Поиск последовательности обхода вершин дерева начиная с листьев.
 * @param graph граф.
 * @param root корень графа.
 * @return очередь обхода вершин.
 */
queue<int> inv_dfs(const vector<TreeNode> &graph, int root) {
    queue<int> result;
    stack<int> st;
    vector<int> flag(graph.size(), 0);
    st.push(root);
    while (!st.empty()) {
        int current = st.top();
        if (graph[current].child.empty() || flag[current] == 1) {
            result.push(current);
            st.pop();
            continue;
        }
        for (int c:graph[current].child) {
            st.push(c);
        }
        flag[current] = 1;
    }
    return result;
}

/**
 * Решение задачи поиска максимального множества независимых вершин в дереве.
 * @param graph граф.
 * @param root корневая вершина этого графа.
 * @return мощность максимального множества независимых вершин.
 */
int solve_independent(const vector<TreeNode> &graph, int root) {
    size_t n = graph.size();
    vector<vector<int>> dp(n);
    for (int i = 0; i < n; ++i) {
        dp[i] = vector<int>(2, 0);
    }
    auto order = inv_dfs(graph, root);
    while (!order.empty()) {
        int current = order.front();
        order.pop();
        if (graph[current].child.empty()) {
            dp[current][0] = 0;
            dp[current][1] = 1;
            continue;
        }
        for (int child:graph[current].child) {
            dp[current][0] += max(dp[child][0], dp[child][1]);
            dp[current][1] += dp[child][0];
        }
        dp[current][1]++;
    }
    return max(dp[root][0], dp[root][1]);
}

int main() {
    unsigned long long int n;
    cin >> n;
    vector<TreeNode> graph(n);
    int root = 0;
    int parent;
    // при чтении корневого дерева - сразу передаем информацию о детишках в вершины дерева
    for (int i = 0; i < n; ++i) {
        cin >> parent;
        if (parent == 0) {
            root = i;
        } else {
            graph[parent - 1].add(i);
        }
    }
    cout << solve_independent(graph, root);
    return 0;
}
