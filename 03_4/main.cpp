/**
 * Введенский Вячеслав В. MADE-ML-22
 *
D. Прямая
Своим уравнением Ax + By + C = 0 задана прямая на плоскости. Требуется найти любую принадлежащую этой прямой точку,
 координаты которой — целые числа от  - 5·1018 до 5·1018 включительно, или выяснить что таких точек нет.
Входные данные
В первой строке содержатся три целых числа A, B и C ( - 2·109 ≤ A, B, C ≤ 2·109) — соответствующие
 коэффициенты уравнения прямой. Гарантируется, что A2 + B2 > 0.
 Выходные данные
Если искомая точка существует, выведите ее координаты, иначе выведите единственное число -1.
 */
#include <iostream>

using namespace std;

/**
 * Расширеный алгоритм Эвклида.
 * @param a число.
 * @param b число.
 * @param x координата x, удовлетворяющая уравнению.
 * @param y координата y, удовлетворяющая уравнению.
 * @return
 */
long long gcd(long long a, long long b, long long &x, long long &y) {
    if (a == 0) {
        x = 0;
        y = 1;
        return b;
    }
    long long x1, y1;
    long long d = gcd(b % a, a, x1, y1);
    x = y1 - (b / a) * x1;
    y = x1;
    return d;
}

/**
 * Нахождение целочисленной точки на прямой Ax+By+C=0.
 * @param a коэффициент.
 * @param b коэффициент.
 * @param c коэффициент.
 * @return пара(флаг успешности, пара(x,y))
 */
pair<bool, pair<long long, long long>> FindPointOnLine(long long a, long long b, long long c) {
    if (c == 0) {
        return make_pair(true, make_pair(0, 0));
    }
    long long x, y;
    auto g = gcd(a, b, x, y);
    if (c % g != 0) {
        return make_pair(false, make_pair(0, 0));
    }
    auto m = -c / g;
    return make_pair(true, make_pair(m * x, m * y));
}

int main() {
    long long a, b, c;
    cin >> a >> b >> c;
    auto res = FindPointOnLine(a, b, c);
    if (!res.first) {
        cout << "-1" << endl;
        return 0;
    }
    auto p = res.second;
    cout << p.first << " " << p.second;
    return 0;
}

