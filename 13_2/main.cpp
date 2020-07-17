/**
 * Введенский Вячеслав В. MADE-ML-22
 *
A. Две окружности. Необходимо выяснить, пересекаются ли заданные окружности и найти точки их пересечения.
 */

#include <iostream>
#include <cmath>
#include <vector>

const long MAX_POINT = static_cast<const long>(1e8);
const double EPS = 1e-8;
using namespace std;

/// точка в целочисленных координатах в 2D
struct LPoint2D {
    LPoint2D(long _x, long _y) : x(_x), y(_y) {}

    LPoint2D() : x(0), y(0) {}

    long x, y;

    bool operator==(const LPoint2D &other) const {
        return x == other.x && y == other.y;
    }
};

/// точка в вещественных координатах в 2D
struct DPoint2D {
    DPoint2D(double _x, double _y) : x(_x), y(_y) {}

    DPoint2D() : x(0), y(0) {}

    double x, y;

    bool operator==(const DPoint2D &other) const {
        return abs(x - other.x) < EPS && abs(y - other.y) < EPS;
    }

    long double mult(const DPoint2D &other) {
        return x * other.y - y * other.x;
    }
};

/// вещественная прямая в 2D
struct DLine2D {
    DLine2D(double _a, double _b, double _c) : a(_a), b(_b), c(_c) {}

    DLine2D() : a(0), b(0), c(0) {}

    double a, b, c;

    double value(const LPoint2D p) const {
        return p.x * a + p.y * b + c;
    }

    double sign(const DPoint2D p) const {
        auto v = p.x * a + p.y * b + c;
        if (v > 0)
            return 1;
        if (v < 0)
            return -1;
        return 0;
    }
};

/// определитель 2*2
long double det(double a11, double a12, double a21, double a22) {
    auto d = a11 * a22 - a12 * a21;
    return d;
}

/// точка пересечения двух прямых (уверены, что они имеют точку пересечения)
DPoint2D cross(const DLine2D &l1, const DLine2D &l2) {
    long double d = det(l1.a, l1.b, l2.a, l2.b);
    auto dx = det(l1.b, l1.c, l2.b, l2.c);
    auto dy = det(l1.c, l1.a, l2.c, l2.a);

    return {static_cast<double>(dx / d), static_cast<double>(dy / d)};
}

/// отсечь многоугольник гиперплоскостью
vector<DPoint2D> cut(const vector<DPoint2D> &points, const DLine2D &hp) {
    vector<DPoint2D> result;
    for (int i = 0; i < points.size(); ++i) {
        auto pi = points[i];
        int i1 = (i + 1) % points.size();
        DPoint2D pi1 = points[i1];
        if (hp.sign(pi) >= 0) {
            result.push_back(pi);
        }
        if (hp.sign(pi) * hp.sign(pi1) < 0) {
            auto _a = pi.y - pi1.y;
            auto _b = pi1.x - pi.x;
            auto len = sqrt(_a * _a + _b * _b);
            _a /= len;
            _b /= len;
            auto _c = (-_a * pi.x - _b * pi.y);// /len;
            DPoint2D p = cross(hp, DLine2D(_a, _b, _c));
            result.push_back(p);
        }
    }
    return result;
}

/// площадь многоугольника
long double computeArea(const vector<DPoint2D> &points) {
    long double area = 0;
    for (int i = 0; i < points.size(); ++i) {
        auto pi = points[i];
        int i1 = (i + 1) % points.size();
        DPoint2D pi1 = points[i1];
        area += pi.mult(pi1);
    }
    return area / 2.;
}

int main() {
    int n;
    cin >> n;
    vector<DPoint2D> points;
    points.emplace_back(-MAX_POINT, -MAX_POINT);
    points.emplace_back(MAX_POINT, -MAX_POINT);
    points.emplace_back(MAX_POINT, MAX_POINT);
    points.emplace_back(-MAX_POINT, MAX_POINT);
    for (int i = 0; i < n; ++i) {
        DLine2D hp;
        cin >> hp.a >> hp.b >> hp.c;
        points = cut(points, hp);
    }
    auto area = computeArea(points);
    cout << fixed;
    cout.precision(8);
    cout << area;
    return 0;
}