/**
 * Введенский Вячеслав В. MADE-ML-22
 *
 Задача D. База в джунглях.
 */

#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <algorithm>

const long MAX_POINT = static_cast<const long>(1e6 + 100);
const long double EPS = 1e-9;
using namespace std;

/// точка в целочисленных координатах в 2D
struct LPoint2D {
    LPoint2D(long long _x, long long _y) : x(_x), y(_y) {}

    LPoint2D() : x(0), y(0) {}

    long long x, y;

    bool operator==(const LPoint2D &other) const {
        return x == other.x && y == other.y;
    }

    LPoint2D &operator=(const LPoint2D &other) = default;
};

/// точка в вещественных координатах в 2D
struct DPoint2D {

    DPoint2D(long double _x, long double _y) : x(_x), y(_y) {}

    DPoint2D() : x(0), y(0) {}

    long double x, y;

    bool operator==(const DPoint2D &other) const {
        return abs(x - other.x) < EPS && abs(y - other.y) < EPS;
    }

    DPoint2D operator+(const DPoint2D &other) const {
        return {x + other.x, y + other.y};
    }

    DPoint2D operator-(const DPoint2D &other) const {
        return {x - other.x, y - other.y};
    }

    long double dist(const DPoint2D &other) const {
        return sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
    }

    long double scalar(const DPoint2D &other) {
        return x * other.x + y * other.y;
    }
};

/// пустая точка для разных дел
static const DPoint2D EMPTY_POINT;

/// вещественная прямая в 2D
struct DLine2D {
    DLine2D(long double _a, long double _b, long double _c) : a(_a), b(_b), c(_c) {}

    DLine2D() : a(0), b(0), c(0) {}

    DLine2D(const DPoint2D &p1, const DPoint2D &p2) {
        a = p1.y - p2.y;
        b = p2.x - p1.x;
        auto len = sqrt(a * a + b * b);
        a /= len;
        b /= len;
        c = (-a * p1.x - b * p1.y);
    }

    DLine2D(const LPoint2D &p1, const LPoint2D &p2) {
        a = p1.y - p2.y;
        b = p2.x - p1.x;
        auto len = sqrt(a * a + b * b);
        a /= len;
        b /= len;
        c = (-a * p1.x - b * p1.y);
    }

    long double a, b, c;

    double sign(const DPoint2D p) const {
        auto v = p.x * a + p.y * b + c;
        if (v > EPS)
            return 1;
        if (v < -EPS)
            return -1;
        return 0;
    }
};

/// определитель 2*2
long double det(long double a11, long double a12, long double a21, long double a22) {
    auto d = a11 * a22 - a12 * a21;
    return d;
}

/// пара(факт пересечения, точка пересечения двух прямых)
pair<bool, DPoint2D> cross(const DLine2D &l1, const DLine2D &l2) {
    long double d = det(l1.a, l1.b, l2.a, l2.b);
    if (abs(d) <= EPS)
        return make_pair(false, EMPTY_POINT);
    auto dx = det(l1.b, l1.c, l2.b, l2.c);
    auto dy = det(l1.c, l1.a, l2.c, l2.a);

    return make_pair(true, DPoint2D{(dx / d), (dy / d)});
}

/// многоугольник из целочисленных точек
struct LPoly2D {
    vector<LPoint2D> points;

    void add(const LPoint2D &p) {
        points.push_back(p);
    }

    int size() const {
        return static_cast<int>(points.size());
    }

    LPoint2D &operator[](int idx) {
        return points[idx];
    }

    const LPoint2D &operator[](int idx) const {
        return points[idx];
    }
};

/// находится ли точка на ограничивающем квадрате
bool onBbox(const DPoint2D &p) {
    return abs(abs(p.x) - MAX_POINT) <= EPS || abs(abs(p.y) - MAX_POINT) <= EPS;
}

/// генерация вектора n полуплоскостей по точкам многоугольника на основании точек i, i+step
vector<DLine2D> generateHalfPlanes(const LPoly2D &poly, int step) {
    vector<DLine2D> result;
    auto psz = poly.size();
    for (int i = 0; i < psz; ++i) {
        auto p1 = poly[i];
        auto p2 = poly[(i + step) % psz];
        result.emplace_back(p1, p2);
    }
    return result;
}

/// перемешать объекты в векторе (почему-то стандартный не захотел работать)
vector<DLine2D> shuff(const vector<DLine2D> &bbox, const vector<DLine2D> &_lines) {
    auto n = _lines.size();
    int idx[n];
    for (int i = 0; i < n; ++i) {
        idx[i] = i;
    }
    shuffle(&idx[0], &idx[0] + n, std::mt19937(std::random_device()()));

    vector<DLine2D> lines(bbox);
    for (int i = 0; i < n; ++i) {
        lines.push_back(_lines[idx[i]]);
    }
    return lines;
}

/// луч - точка и направление.
struct Ray2D {
    Ray2D(DPoint2D _p, DPoint2D _dir, bool _positive) : p(_p), dir(_dir), positive(_positive) {}

    Ray2D() {}

    DPoint2D p;
    DPoint2D dir;
    bool positive{};

    bool later(const Ray2D &other) {
        DPoint2D v2 = other.p - p;
        auto d = dir.scalar(v2);
        return d < 0;
    }
};

/// найти минимальную точку пересечения полуплоскостей и прмой с индексом last.
pair<bool, DPoint2D> findCrossPointMinY(const vector<DLine2D> &lines, int last) {
    vector<Ray2D> rays;
    auto lastLine = lines[last];
    DPoint2D v1(-lastLine.b, lastLine.a);
    DPoint2D v2(lastLine.b, -lastLine.a);
    Ray2D ray_pos;
    Ray2D ray_neg;
    for (int i = 0; i < last; ++i) {
        auto res = cross(lastLine, lines[i]);
        if (!res.first) {
            continue;
        }
        DPoint2D candidate = res.second;
        auto sgn1 = lines[i].sign(candidate + v1);
//        auto sgn2 = lines[i].sign(candidate + v2);
        if (sgn1 > 0) {
            ray_pos = Ray2D(candidate, v1, true);
            rays.push_back(ray_pos);
        } else {
            ray_neg = {candidate, v2, false};
            rays.push_back(ray_neg);
        }
    }
    for (int i = 0; i < rays.size(); ++i) {
        auto ray = rays[i];
        if (ray.positive) {
            if (ray.later(ray_pos))
                ray_pos = ray;
        } else {
            if (ray.later(ray_neg))
                ray_neg = ray;
        }
    }
    if (ray_pos.later(ray_neg)) {
        return make_pair(false, EMPTY_POINT);
    }
    if (ray_neg.p.dist(ray_pos.p) <= EPS)
        return make_pair(false, EMPTY_POINT);
    if (ray_pos.p.y < ray_neg.p.y) {
        return make_pair(true, ray_pos.p);
    }
    return make_pair(true, ray_neg.p);
}

/// провека существования точки удовлетворяющей всем полуплоскостям
pair<bool, DPoint2D> checkCuts(const vector<DLine2D> &lines) {
    DPoint2D p(-MAX_POINT, -MAX_POINT);
    auto lsz = lines.size();
    for (int i = 0; i < lsz; ++i) {
        if (lines[i].sign(p) < 0) {
            auto res = findCrossPointMinY(lines, i);
            if (res.first) {
                p = res.second;
            } else {
                return make_pair(false, EMPTY_POINT);
            }
        }
    }
    return make_pair(true, p);
}

int main() {
    std::ios::sync_with_stdio(false), std::cin.tie(0), std::cout.tie(0);
    LPoly2D poly, bboxPoly;

    bboxPoly.add({-MAX_POINT, -MAX_POINT});
    bboxPoly.add({MAX_POINT, -MAX_POINT});
    bboxPoly.add({MAX_POINT, MAX_POINT});
    bboxPoly.add({-MAX_POINT, MAX_POINT});
    auto bbox = generateHalfPlanes(bboxPoly, 1);

    int n;
    cin >> n;
    vector<LPoint2D> points;
    for (int i = 0; i < n; ++i) {
        long x, y;
        cin >> x >> y;
        points.emplace_back(x, y);
    }
    for (int i = n - 1; i >= 0; i--) {
        poly.add(points[i]);
    }

    int l = 1, r = n;

    int idx = (l + r) / 2;
    int lastSuccess = 0;
    while (l <= r) {
        auto lines = generateHalfPlanes(poly, idx + 1);
        lines = shuff(bbox, lines);
        auto res = checkCuts(lines);
        if (!res.first || onBbox(res.second)) {
            r = idx - 1;
        } else {
            lastSuccess = idx;
            l = idx + 1;
        }
        idx = (l + r) / 2;
    }
    cout << lastSuccess + 1 << endl;
    return 0;
}
