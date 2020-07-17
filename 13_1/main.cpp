/**
 * Введенский Вячеслав В. MADE-ML-22
 *
A. Две окружности. Необходимо выяснить, пересекаются ли заданные окружности и найти точки их пересечения.
 */

#include <iostream>
#include <cmath>

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

    LPoint2D operator-(const LPoint2D &other) const {
        return {x - other.x, y - other.y};
    }

};

/// окружность с целочисленными параметрами в 2D
struct Circle2D {
    Circle2D() : r(0) {}

    Circle2D(const LPoint2D &_p, long _r) : p(_p), r(_r) {}

    Circle2D(long _x, long _y, long _r) : p(_x, _y), r(_r) {}

    LPoint2D p;
    long r;
};

/// случай совпадения центров окружностей
bool solveEqCenter(const Circle2D &A, const Circle2D &B) {
    if (A.p == B.p) {
        if (A.r == B.r) { // бесконечно много точек
            cout << "3" << "\n";
            return true;
        }
        cout << "0" << "\n"; // 0 точек
        return true;
    }
    return false;
}

/// решение проблемы пересечения окружностей
void solveCircleCrossProblem(const Circle2D &_A, const Circle2D &_B) {
    if (solveEqCenter(_A, _B)) {
        return;
    }
    LPoint2D shift(_A.p);
    Circle2D A0(0, 0, _A.r), B0(_B.p - _A.p, _B.r);

    long long a = 2 * B0.p.x;
    long long b = 2 * B0.p.y;
    long long c = -B0.p.x * B0.p.x - B0.p.y * B0.p.y + B0.r * B0.r - A0.r * A0.r;
    long long a2b2 = a * a + b * b;
    long double d0 = abs(c) / sqrt(a2b2);
    if (c * c > A0.r * A0.r * a2b2 + EPS) {
        cout << "0" << "\n"; // 0 точек
        return;
    }
    long double x0 = -a * c / (long double) a2b2;
    long double y0 = -b * c / (long double) a2b2;
    if (abs(c * c - A0.r * A0.r * a2b2) < EPS) {
        cout << "1" << "\n"; // 1 точка
        cout << fixed;
        cout.precision(8);
        cout << x0 + shift.x << " " << y0 + shift.y << "\n";
        return;
    }

    long double d2 = A0.r * A0.r - c * c / (long double) a2b2;
    long double m = sqrt(d2 / (long double) a2b2);
    long double x1 = x0 + m * b;
    long double y1 = y0 - m * a;

    long double x2 = x0 - m * b;
    long double y2 = y0 + m * a;

    cout << "2" << "\n"; // 1 точка
    cout << fixed;
    cout.precision(8);
    cout << x0 + shift.x << " " << y0 + shift.y << "\n";
    cout << d0 << " " << sqrt(d2) << "\n";
    cout << x1 + shift.x << " " << y1 + shift.y << "\n";
    cout << x2 + shift.x << " " << y2 + shift.y << "\n";
}

int main() {
    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        Circle2D A, B;
        cin >> A.p.x >> A.p.y >> A.r;
        cin >> B.p.x >> B.p.y >> B.r;
        solveCircleCrossProblem(A, B);
    }
    cout << endl;
    return 0;
}