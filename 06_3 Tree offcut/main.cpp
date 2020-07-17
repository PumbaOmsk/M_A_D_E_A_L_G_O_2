/**
 * Введенский Вячеслав В. MADE-ML-22
 *
C. Окна
На экране расположены прямоугольные окна, каким-то образом перекрывающиеся. Вам необходимо найти точку,
 которая покрыта наибольшим числом из них.
 Входные данные: В первой строке входного файла записано число окон n (1 ≤ n ≤ 50000). Следующие n строк содержат
 координаты окон x(1, i) y(1, i) x(2, i) y(2, i), где (x(1, i), y(1, i)) — координаты левого верхнего угла i-го окна, а
 (x(2, i), y(2, i)) — правого нижнего (на экране компьютера y растет сверху вниз, а x — слева направо).
 Все координаты — целые числа, по модулю не превосходящие 2·10^5.
Выходные данные: В первой строке выходного файла выведите максимальное число окон, покрывающих какую-либо из точек в
 данной конфигурации. Во второй строке выведите два целых числа, разделенные пробелом — координаты точки, покрытой
 максимальным числом окон. Окна считаются замкнутыми, т.е. покрывающими свои граничные точки.
 */
#include <vector>
#include <iostream>
#include <set>

using namespace std;
/**
 * Максимальное, неопределенное значение.
 */
const long INF = static_cast<long>(1e9);

/**
 * Получить элемент из дерева.
 * @param T вектор дерева отрезков.
 * @param updAdd вектор операций изменения (add).
 * @param v номер вершины.
 * @return элемент в вершине.
 */
long get(const vector<long> &T, const vector<long> &updAdd, int v) {
    return T[v] + updAdd[v];
}

/**
 * Протолкнуть изменения ниже.
 * @param l левая граница.
 * @param r правая граница.
 */
void push(vector<long> &T, vector<long> &y, vector<long> &updAdd, int v, int l, int r) {
    if (l == r) {
        T[v] += updAdd[v];
        y[v] = l;
    } else {

        updAdd[2 * v + 1] += updAdd[v];
        updAdd[2 * v + 2] += updAdd[v];

        auto left = get(T, updAdd, 2 * v + 1);
        auto right = get(T, updAdd, 2 * v + 2);
        if (left > right) {
            T[v] = left;
            y[v] = y[2 * v + 1];
        } else {
            T[v] = right;
            y[v] = y[2 * v + 2];
        }
    }
    updAdd[v] = 0;
}

/**
 * групповая операция обновления - добавить значение на отрезке.
 * @param x значение
 */
void updateAdd(vector<long> &T, vector<long> &y, vector<long> &updAdd, int v, int l, int r,
               int a, int b, long x) {
    push(T, y, updAdd, v, l, r);
    if (l > b || r < a) {
        return;
    }
    if (l >= a && r <= b) {
        updAdd[v] += x;
        return;
    }
    int m = (l + r) >> 1;
    updateAdd(T, y, updAdd, 2 * v + 1, l, m, a, b, x);
    updateAdd(T, y, updAdd, 2 * v + 2, m + 1, r, a, b, x);
    auto left = get(T, updAdd, 2 * v + 1);
    auto right = get(T, updAdd, 2 * v + 2);
    if (left > right) {
        T[v] = left;
        y[v] = y[2 * v + 1];
    } else {
        T[v] = right;
        y[v] = y[2 * v + 2];
    }
}

/**
 * вернуть минимальный элемент на подотрезке, и значение координаты отрезка на котором оно достигается.
 * @param a левая граница исходного отрезка.
 * @param b правая граница исходного отрезка.
 * @return минимальный элемент, значение координаты.
 */
pair<long, long> rmq(vector<long> &T, vector<long> &y, vector<long> &updAdd, int v, int l, int r, int a, int b) {
    push(T, y, updAdd, v, l, r);
    if (l > b || r < a) {
        return make_pair(INF, INF);
    }
    if (l >= a && r <= b) {
        return make_pair(get(T, updAdd, v), y[v]);
    }
    int m = (l + r) >> 1;
    auto left = rmq(T, y, updAdd, 2 * v + 1, l, m, a, b);
    auto right = rmq(T, y, updAdd, 2 * v + 2, m + 1, r, a, b);
    if (left.first > right.first) {
        return left;
    } else {
        return right;
    }
}

/// прямоугольник
struct Rect {
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
};

/// по возрастанию левой границы
struct ComparatorLeft {
    bool operator()(const Rect &r1, const Rect &r2) const {
        return r1.x1 < r2.x1;
    }
};

/// по возрастанию правой границы
struct ComparatorRight {
    bool operator()(const Rect &r1, const Rect &r2) const {
        return r1.x2 < r2.x2;
    }
};

/**
 * Поиск точки максимального пересечения окнами.
 * @param r вектор окон-прямоугольников.
 * @return пара (максимальное пересечение, (x,y))
 */
pair<int, pair<long, long>> FindIntersectionMax(const vector<Rect> &r) {
    multiset<Rect, ComparatorLeft> left_rects;
    multiset<Rect, ComparatorRight> right_rects;
    set<int> points; // множество точек по оси X по возрастанию
    for (auto &&rect:r) {
        left_rects.emplace(rect);
        right_rects.emplace(rect);
        points.insert(rect.x1);
        points.insert(rect.x2 + 1); // нам нужна вертикаль следующая за прямоугольником.
    }
    int bound = 524288;  // степень двойки в которую влезет 4*10^5
    int shift = bound >> 1; // смещение, чтобы обрабатывать отрицательные координаты.
    int size = 2 * bound - 1; // размер под дерево.
    vector<long> T(size, 0); // дерево.
    vector<long> updAdd(size, 0); // отложенное обновление.
    vector<long> y(size, INF); // значение координаты в дереве, на которой достигается текущее значение.
    for (int i = 0; i < bound; ++i) {
        y[bound - 1 + i] = i;
    }
    for (int v = bound - 2; v >= 0; v--) {
        y[v] = y[2 * v + 1];
    }

    int mx = 0;
    long max_x = 0;
    long max_y = 0;

    for (int x:points) {
        if (right_rects.empty() && left_rects.empty()) {
            break;
        }
        // начался прямоугольник
        while (!left_rects.empty() && x == left_rects.begin()->x1) {
            auto y1 = left_rects.begin()->y1;
            auto y2 = left_rects.begin()->y2;
            updateAdd(T, y, updAdd, 0, 0, bound - 1, shift + y1, shift + y2, 1);
            left_rects.erase(left_rects.begin());
        }
        // закончился прямоугольник
        while (!right_rects.empty() && x - 1 == right_rects.begin()->x2) {
            auto y1 = right_rects.begin()->y1;
            auto y2 = right_rects.begin()->y2;
            updateAdd(T, y, updAdd, 0, 0, bound - 1, shift + y1, shift + y2, -1);
            right_rects.erase(right_rects.begin());
        }
        // текущий максимум наложений прямоугольников.
        auto res = rmq(T, y, updAdd, 0, 0, bound - 1, 0, bound - 1);
        if (res.first > mx) {
            mx = res.first;
            max_y = res.second - shift;
            max_x = x;
        }
    }
    return make_pair(mx, make_pair(max_x, max_y));
}

int main() {
    int n;
    cin >> n;
    vector<Rect> r(n);
    for (int i = 0; i < n; ++i) {
        cin >> r[i].x1 >> r[i].y1 >> r[i].x2 >> r[i].y2;
    }

    auto res = FindIntersectionMax(r);
    cout << res.first << endl;
    cout << res.second.first << " " << res.second.second << endl;
    return 0;
}