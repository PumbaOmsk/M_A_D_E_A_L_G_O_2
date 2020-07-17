/**
 * Введенский Вячеслав В. MADE-ML-22
 *
E. Покрытие кругом. N точек на плоскости. Найдите круг минимального радиуса, который покрывает все заданные точки.
 */

#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include <algorithm>

const long double EPS = 1e-9;
using namespace std;

struct DPoint2D;
struct LPoint2D {
    LPoint2D(long long _x, long long _y) : x(_x), y(_y) {}

    LPoint2D() : x(0), y(0) {}

    long long x, y;

    bool operator==(const LPoint2D &other) const {
        return x == other.x && y == other.y;
    }

    LPoint2D &operator=(const LPoint2D &other) = default;

    long double dist(const LPoint2D &other) const {
        return sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
    }

    long long mult(const LPoint2D &other) const {
        return x * other.y - y * other.x;
    }
    LPoint2D operator+(const LPoint2D &other) const {
        return {x+other.x,y+other.y};
    }
    friend DPoint2D operator / (const LPoint2D &left,long double dev) ;
    LPoint2D operator-(const LPoint2D &other) const {
        return {x-other.x,y-other.y};
    }

};

struct DPoint2D {
    DPoint2D(long double _x, long double _y) : x(_x), y(_y) {}

    DPoint2D() : x(0), y(0) {}

    long double x, y;

    bool operator==(const DPoint2D &other) const {
        return abs(x - other.x) < EPS && abs(y - other.y) < EPS;
    }

    long double dist(const LPoint2D &other) const {
        return sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
    }
    DPoint2D operator+(const DPoint2D &other) const {
        return {x+other.x,y+other.y};
    }
    DPoint2D operator-(const DPoint2D &other) const {
        return {x-other.x,y-other.y};
    }


};
DPoint2D operator / (const LPoint2D &left,long double dev){
    return {left.x/dev,left.y/dev};
}

DPoint2D center(const LPoint2D &p1, const LPoint2D &p2) {
    return (p1 + p2) / 2.;
}

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

    DLine2D(const LPoint2D &p1, const LPoint2D &p2) {
        a = p1.y - p2.y;
        b = p2.x - p1.x;
        auto len = sqrt(a * a + b * b);
        a /= len;
        b /= len;
        c = (-a * p1.x - b * p1.y);
    }

    long double a, b, c;
};

long double det(long double a11, long double a12, long double a21, long double a22) {
    auto d = a11 * a22 - a12 * a21;
    return d;
}

DPoint2D cross(const DLine2D &l1, const DLine2D &l2) {
    long double d = det(l1.a, l1.b, l2.a, l2.b);
    auto dx = det(l1.b, l1.c, l2.b, l2.c);
    auto dy = det(l1.c, l1.a, l2.c, l2.a);

    return {(dx / d), (dy / d)};
}

struct DCircle2D {
    DCircle2D() : r(0) {}

    DCircle2D(long double _x, long double _y, long double _r) : p(_x, _y), r(_r) {}

    DCircle2D(const DPoint2D _p, long double _r) : p(_p), r(_r) {}

    DPoint2D p;
    long double r;

    int sign(const LPoint2D &other) {
        auto d2 = (p.x - other.x) * (p.x - other.x) + (p.y - other.y) * (p.y - other.y);
        if (d2 > r * r + EPS)
            return 1;
        if (d2 < r * r - EPS)
            return -1;
        return 0;
    }

};

DCircle2D minCircle(const LPoint2D &p1, const LPoint2D &p2) {
    long double dist = p1.dist(p2);
    return {(p1 + p2) / 2., dist / 2};
}

bool atOneLine(const LPoint2D &p1, const LPoint2D &p3, const LPoint2D &p2) {
    LPoint2D p12(p2-p1);
    LPoint2D p13(p3-p1);
    return p12.mult(p13) == 0;
}

DCircle2D minCircle(const LPoint2D &p1, const LPoint2D &p2, const LPoint2D &p3) {
    if (atOneLine(p1, p2, p3)) {
        auto c = minCircle(p1, p3);
        if (c.sign(p2) > 0) {
            c = minCircle(p1, p2);
            if (c.sign(p3) > 0) {
                c = minCircle(p2, p3);
            }
        }
        return c;
    }

    DLine2D l1(p1, p2);
    DPoint2D pc12 = center(p1, p2);
    DLine2D lc1(pc12, DPoint2D(pc12.x + l1.a, pc12.y + l1.b));

    DLine2D l2(p2, p3);
    DPoint2D pc23 = center(p2, p3);
    DLine2D lc2(pc23, DPoint2D(pc23.x + l2.a, pc23.y + l2.b));

    auto c = cross(lc1, lc2);
    auto r = c.dist(p1);
    return {c, r};

//    auto c12 = cross(lc1, lc2);
//    DLine2D l3(p1, p3);
//    DPoint2D pc13 = center(p1, p3);
//
//    DLine2D lc3(pc13, DPoint2D(pc13.x + l3.a, pc13.y + l3.b));
//    auto c13 = cross(lc1, lc3);
//    auto c23 = cross(lc2, lc3);
//    DPoint2D c((c12.x+c13.x+c23.x)/3.,(c12.y+c13.y+c23.y)/3.);
//    auto r1 = c.dist(p1);
//    auto r2 = c.dist(p2);
//    auto r3 = c.dist(p3);
//    return {c, (r1+r2+r3)/3.};
}

DCircle2D minDiskWith2Point(const vector<LPoint2D> &points, int prev, int last) {
    DCircle2D c = minCircle(points[prev], points[last]);
    for (int i = 0; i < last; ++i) {
        if (c.sign(points[i]) > 0) {
            c = minCircle(points[i], points[prev], points[last]);
        }
    }
    return c;
}

DCircle2D minDiskWith1Point(const vector<LPoint2D> &points, int last) {
    DCircle2D c = minCircle(points[0], points[last]);
    for (int i = 1; i < last; ++i) {
        if (c.sign(points[i]) > 0) {
            c = minDiskWith2Point(points, last, i);
        }
    }
    return c;
}

vector<LPoint2D> shuff(const vector<LPoint2D> &_points) {
    auto n = _points.size();
    int idx[n];
    for (int i = 0; i < n; ++i) {
        idx[i] = i;
    }
    shuffle(&idx[0], &idx[0] + n, std::mt19937(std::random_device()()));

    vector<LPoint2D> points;
    for (int i = 0; i < n; ++i) {
        points.push_back(_points[idx[i]]);
    }
    return points;
}

DCircle2D minDisk(const vector<LPoint2D> &_points) {
    auto points = shuff(_points);
    DCircle2D c = minCircle(points[0], points[1]);
    for (int i = 2; i < points.size(); ++i) {
        if (c.sign(points[i]) > 0) {
            c = minDiskWith1Point(points, i);
        }
    }
    return c;
}

int main() {
    int n;
    cin >> n;
    vector<LPoint2D> points(n);
    for (int i = 0; i < n; ++i) {
        cin >> points[i].x >> points[i].y;
    }
    DCircle2D c = minDisk(points);
    cout << fixed;
    cout.precision(10);
    cout << c.p.x << " " << c.p.y << "\n";
    cout << c.r << endl;
    return 0;
}
