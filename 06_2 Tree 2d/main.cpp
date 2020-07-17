/**
 * Введенский Вячеслав В. MADE-ML-22
 *

 */
#include <iostream>
#include <vector>
#include <random>

using namespace std;

// хранение информации об обновлении во внутренних деревьях.
struct Node {
    double localValue = 0;
    double globalValue = 0;
    double localLazy = 0;
    double globalLazy = 0;
};

// дерево отрезков одномерное.
struct SegmentTree {
    int n;
    int bound = 1;
    int size = 0;
    vector<Node> T;

    SegmentTree() = default;

    void init(int n_) {
        n = n_;
        while (bound < n) {
            bound = bound << 1;
        }
        size = 2 * bound - 1;
        T = vector<Node>(size);
    }

    Node &operator[](int i) {
        return T[bound - 1 + i];
    }

    double rsq(int v, int l, int r, int x1, int y1, int x2, int y2, double lazy) {
        return rsq(v, l, r, 0, bound - 1, x1, y1, x2, y2, lazy);
    }

    void update(int v, int dx, int x1, int y1, int x2, int y2, double dw, bool covered) {
        update(v, dx, 0, bound - 1, x1, y1, x2, y2, dw, covered);
    }

private:
    void update(int v, int dx, int yl, int yr, int x1, int y1, int x2, int y2, double dw, bool covered) {
        if (y1 <= yl && yr <= y2) {
            if (covered) {
                T[v].globalLazy += dw;
                T[v].globalValue += dw * dx * (yr - yl + 1);
            } else {
                T[v].localLazy += dw;
                T[v].localValue += dw * dx * (yr - yl + 1);
            }
            return;
        }
        if (yr < y1 || y2 < yl)
            return;
        int ym = (yl + yr) >> 1;

        auto left = 2 * v + 1;
        auto right = 2 * v + 2;

        update(left, dx, yl, ym, x1, y1, x2, y2, dw, covered);
        update(right, dx, ym + 1, yr, x1, y1, x2, y2, dw, covered);

        T[v].localValue = T[left].localValue + T[right].localValue + (T[v].localLazy) * dx * (yr - yl + 1);
        T[v].globalValue = T[left].globalValue + T[right].globalValue + (T[v].globalLazy) * dx * (yr - yl + 1);
    }

    double rsq(int v, int xl, int xr, int yl, int yr, int x1, int y1, int x2, int y2, double lazy) {
        if (y1 <= yl && yr <= y2) {
            if (x1 <= xl && xr <= x2) {
                return T[v].localValue + T[v].globalValue + lazy * (xr - xl + 1) * (yr - yl + 1);
            }
            if (xl > x2 || xr < x1)
                return 0;
            double scaled_value = T[v].globalValue * 1.0 * (x2 - x1 + 1) / (xr - xl + 1);
            return scaled_value + lazy * (x2 - x1 + 1) * (yr - yl + 1);
        }
        if (yl > y2 || yr < y1)
            return 0;
        int ym = (yl + yr) >> 1;
        auto left = 2 * v + 1;
        auto right = 2 * v + 2;
        if (x1 <= xl && xr <= x2) {

            return rsq(left, xl, xr, yl, ym, x1, y1, x2, y2, lazy + T[v].globalLazy + T[v].localLazy) +
                   rsq(right, xl, xr, ym + 1, yr, x1, y1, x2, y2, lazy + T[v].globalLazy + T[v].localLazy);
        }
        if (xl > x2 || xr < x1)
            return 0;

        return rsq(left, xl, xr, yl, ym, x1, y1, x2, y2, lazy + T[v].globalLazy) +
               rsq(right, xl, xr, ym + 1, yr, x1, y1, x2, y2, lazy + T[v].globalLazy);
    }
};

// двумерное дерево отрезков.
struct SegmentTree2d {
    int nx;
    int ny;
    int bound = 1;       // под хранение дерева
    long long size = 0;  // реальный размер
    vector<SegmentTree> T;

    explicit SegmentTree2d(int x, int y) {
        nx = x;
        ny = y;
        while (bound < nx) {
            bound = bound << 1;
        }
        size = 2 * bound - 1;
        T = vector<SegmentTree>(size);
        for (int i = 0; i < size; ++i) {
            T[i].init(ny);
        }
    }

    // обновить в прямоугольнике
    void update(int x1, int y1, int x2, int y2, long long dw) {
        update(0, 0, bound - 1, x1, y1, x2, y2, dw);
    }

    // запрос суммы из прямоугольника
    long long rsq(int x1, int y1, int x2, int y2) {
        return (long long) rsq(0, 0, bound - 1, x1, y1, x2, y2);
    }

private:

    double rsq(int v, int l, int r, int x1, int y1, int x2, int y2) {
        if (x1 <= l && r <= x2) {
            return T[v].rsq(0, l, r, x1, y1, x2, y2, 0);
        }
        if (l > x2 || r < x1)
            return 0;
        int m = (l + r) >> 1;
        int tx1 = max(x1, l);
        int tx2 = min(x2, r);
        return T[v].rsq(0, l, r, tx1, y1, tx2, y2, 0) +
               rsq(2 * v + 1, l, m, x1, y1, x2, y2) +
               rsq(2 * v + 2, m + 1, r, x1, y1, x2, y2);

    }

    void update(int v, int l, int r, int x1, int y1, int x2, int y2, long long dw) {
        if (x1 <= l && r <= x2) {
            T[v].update(0, r - l + 1, x1, y1, x2, y2, dw, true);
            return;
        }
        if (l > x2 || r < x1) {
            return;
        }
        int m = (l + r) >> 1;
        update(2 * v + 1, l, m, x1, y1, x2, y2, dw);
        update(2 * v + 2, m + 1, r, x1, y1, x2, y2, dw);

        int tx1 = max(x1, l);
        int tx2 = min(x2, r);
        double sdw = dw * (tx2 - tx1 + 1) / (double) (r - l + 1);
        T[v].update(0, r - l + 1, tx1, y1, tx2, y2, sdw, false);
    }
};

int main() {
    int x, y;
    cin >> x >> y;
    int n;
    cin >> n;

    SegmentTree2d tree(x, y);

    for (int row = 0; row < n; ++row) {
        int command, x1, y1, x2, y2;;
        cin >> command >> x1 >> y1 >> x2 >> y2;
        if (command == 1) {
            long w;
            cin >> w;
            tree.update(x1 - 1, y1 - 1, x2 - 1, y2 - 1, w);
        } else {
            cout << tree.rsq(x1 - 1, y1 - 1, x2 - 1, y2 - 1) << endl;
        }
    }
    return 0;
}