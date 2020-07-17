/**
 * Введенский Вячеслав В. MADE-ML-22
 *
C. Площади. Даны n прямых на плоскости. Они делят плоскость на части, некоторые из которых конечны,
 некоторые — бесконечны. Найдите площади всех конечных частей.
 */

#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <algorithm>

/// размер ограничивающего квадрата.
const long MAX_POINT = static_cast<const long>(1e8);
const long double MIN_AREA = static_cast<const long double>(1e-8);
const double EPS = 1e-10;
using namespace std;

/// точка в вещественных координатах в 2D
struct DPoint2D {
    DPoint2D(long double _x, long double _y) : x(_x), y(_y) {}

    DPoint2D() : x(0), y(0) {}

    long double x, y;

    bool operator==(const DPoint2D &other) const {
        return abs(x - other.x) < EPS && abs(y - other.y) < EPS;
    }

    long double mult(const DPoint2D &other) const {
        return x * other.y - y * other.x;
    }
};

/// вещественная прямая в 2D
struct DLine2D {
    DLine2D(double _a, double _b, double _c) : a(_a), b(_b), c(_c) {}

    DLine2D() : a(0), b(0), c(0) {}

    DLine2D(const DPoint2D &p1, const DPoint2D &p2) {
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

/// точка пересечения двух прямых
DPoint2D cross(const DLine2D &l1, const DLine2D &l2) {
    long double d = det(l1.a, l1.b, l2.a, l2.b);
    auto dx = det(l1.b, l1.c, l2.b, l2.c);
    auto dy = det(l1.c, l1.a, l2.c, l2.a);

    return {static_cast<double>(dx / d), static_cast<double>(dy / d)};
}

/// многоугольник из вещественных точек
struct DPoly2D {
    vector<DPoint2D> points;

    void add(const DPoint2D &p) {
        points.push_back(p);
    }

    int size() const {
        return static_cast<int>(points.size());
    }

    DPoint2D &operator[](int idx) {
        return points[idx];
    }

    const DPoint2D &operator[](int idx) const {
        return points[idx];
    }
};

/// разделить полигон на два прямой
pair<DPoly2D, DPoly2D> cut(const DPoly2D &poly, const DLine2D &line) {
    DPoly2D result1;
    DPoly2D result2;
    auto psz = poly.size();
    for (int i = 0; i < psz; ++i) {
        auto p1 = poly[i];
        int i1 = (i + 1) % psz;
        DPoint2D p2 = poly[i1];
        auto lsgn1 = line.sign(p1);
        if (lsgn1 >= 0) {
            result1.add(p1);
        }
        if (lsgn1 <= 0) {
            result2.add(p1);
        }

        if (lsgn1 * line.sign(p2) < 0) {
            auto _a = p1.y - p2.y;
            auto _b = p2.x - p1.x;
            auto len = sqrt(_a * _a + _b * _b);
            _a /= len;
            _b /= len;
            auto _c = (-_a * p1.x - _b * p1.y);// /len;
            DPoint2D p = cross(line, DLine2D(_a, _b, _c));
            result1.add(p);
            result2.add(p);
        }
    }
    return make_pair(result1, result2);
}

/// порезать вектор полигонов прямой
vector<DPoly2D> cutAll(const vector<DPoly2D> &areas, const DLine2D &line) {
    vector<DPoly2D> result;
    for (int i = 0; i < areas.size(); ++i) {
        auto ps = cut(areas[i], line);
        if (ps.first.size() > 2)
            result.push_back(ps.first);
        if (ps.second.size() > 2)
            result.push_back(ps.second);
    }
    return result;
}

/// посчитать площадь полигона
long double computeArea(const DPoly2D &area) {
    long double sq = 0;
    auto psz = area.size();
    for (int i = 0; i < psz; ++i) {
        auto pi = area[i];
        int i1 = (i + 1) % psz;
        DPoint2D pi1 = area[i1];
        sq += pi.mult(pi1);
    }
    return sq / 2.;
}

/// порезать вектор полигонов всеми прямыми по очереди
vector<DPoly2D> findManyCrossAreas(const vector<DPoly2D> &areas, const vector<DLine2D> &lines) {
    vector<DPoly2D> result(areas);
    for (int i = 0; i < lines.size(); ++i) {
        result = cutAll(result, lines[i]);
    }
    return result;
}

/// находится ли точка на ограничивающем квадрате
bool onBbox(const DPoint2D &p) {
    return abs(abs(p.x) - MAX_POINT) <= EPS || abs(abs(p.y) - MAX_POINT) <= EPS;
}

/// являет ли полигон бесконечным (пересекает ограничивающий)
bool crossBbox(const DPoly2D &area) {
    for (int i = 0; i < area.size(); ++i) {
        if (onBbox(area[i]))
            return true;
    }
    return false;
}

/// удалить из вектора полигонов бесконечные
vector<DPoly2D> filter(const vector<DPoly2D> &areas) {
    vector<DPoly2D> result;
    for (int i = 0; i < areas.size(); ++i) {
        if (!crossBbox(areas[i])) {
            result.push_back(areas[i]);
        }
    }
    return result;
}

int main() {
    vector<DPoly2D> areas;

    DPoly2D poly;
    poly.add({-MAX_POINT, -MAX_POINT});
    poly.add({MAX_POINT, -MAX_POINT});
    poly.add({MAX_POINT, MAX_POINT});
    poly.add({-MAX_POINT, MAX_POINT});
    areas.push_back(poly);

    vector<DLine2D> lines;
    int n;
    cin >> n;
    for (int i = 0; i < n; ++i) {
        DPoint2D a, b;
        cin >> a.x >> a.y >> b.x >> b.y;
        lines.emplace_back(a, b);
    }
    areas = findManyCrossAreas(areas, lines);
    areas = filter(areas);
    vector<long double> sq;
    for (int i = 0; i < areas.size(); ++i) {
        long double s = computeArea(areas[i]);
        if (s >= MIN_AREA) {
            sq.push_back(s);
        }
    }
    sort(sq.begin(), sq.end());
    cout << fixed;
    cout.precision(8);
    cout << sq.size() << "\n";
    for (auto s:sq) {
        cout << s << "\n";
    }
    cout << endl;
    return 0;
}