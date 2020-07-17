/**
 * Введенский Вячеслав В. MADE-ML-22
 *
 C. Корень по модулю
Даны числа a, b и m, (1≤a,b<m≤109), m — простое. Требуется найти корень степени b из a по модулю m.
Входные данные: Входной файл состоит из одного или нескольких тестов. Количество тестов T≤300 задано в первой строке.
В T последующих строках заданы по три числа в каждой — a, b и m.
Выходные данные: Для каждого из тестов выведите какой-либо из корней степени b из a по модулю m,
 либо -1, если искомого корня не существует. Выводимое число должно быть в пределах от -1 до m−1.
 */
#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <unordered_map>

using namespace std;

/**
 * Возведение в степень по модулю.
 * @param base
 * @param d
 * @param mod
 * @return
 */
long long ModPow(long long base, long long d, long long mod) {
    if (mod == 1)
        return 0;
    long long m = 1;
    base = base % mod;
    while (d > 0) {
        if (d % 2 == 1)
            m = (m * base) % mod;
        d = d >> 1;
        base = (base * base) % mod;
    }
    return m;
}

/**
*  Дискретный логарифм (сначала baby, потом giant).
* @param a основание
* @param b число
* @param p модуль
*/
long long DiscreteLogBG(long long a, long long b, long long p) {
    long long n = 1 + (long long) (sqrt(p));

    unordered_map<long long, long long> m;
//    map<long long, long long> m;
    long long bstep = 1;
    for (long long r = 0; r <= n; ++r) {
            m[bstep] = r;
        bstep = a * bstep % p;
    }

    long long gfactor = ModPow(a, (p - 2) * n, p);
    long long gstep = b;
    for (long long q = 0; q <= n; ++q) {
        if (m.count(gstep) > 0) {
            long long i = m[gstep];
            auto res = q * n + i;
            if (res < p) {
                return res;
            }
        }
        gstep = gstep * gfactor % p;
    }
    return -1;
}

/**
 * Проверка корня на первообразность.
 * @param g кандидат.
 * @param p модуль.
 * @param base множители (основание факторизации).
 * @param last количество множителей.
 * @return true, если корень первообразный.
 */
bool CheckRoot(long g, long p, const vector<long> &base, int last) {
    for (int i = 0; i < last; ++i) {
        long d = (p - 1) / base[i];
        if (ModPow(g, d, p) == 1)
            return false;
    }
    return true;
}

/**
 * Вернуть минимальный первообразный корень по модулю p
 * @param p модуль.
 * @return минимальный первообразный корень.
 */
long long PrimitiveRootsMin(long p) {
    vector<long> base(10, 0);
    int last = 0;
    long n = p - 1;
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) {
            base[last++] = i;
            while (n % i == 0)
                n /= i;
        }
    }
    if (n > 1)
        base[last++] = n;

    for (int g = 2; g <= p; ++g) {
        if (CheckRoot(g, p, base, last)) {
            return g;
        }
    }
    return -1;
}

/**
 * Нахождение любого корня по модулю. x^b=a mod m
 * @param a значение.
 * @param b степень.
 * @param m модуль.
 * @return значение корня.
 */
long long FindRootMod(long long a, long long b, long long m) {
    auto g = PrimitiveRootsMin(static_cast<long>(m));
    if (g == -1)
        return -1;
    auto gb = ModPow(g, b, m);
    auto l = DiscreteLogBG(gb, a, m);
    if (l == -1)
        return -1;
    return ModPow(g, l, m);
}

int main() {
    int T;
    cin >> T;
    for (int i = 0; i < T; ++i) {
        long a, b, m;
        cin >> a >> b >> m;
        cout << FindRootMod(a, b, m) << endl;
    }
    return 0;
}
//#include <chrono>
//#include <iostream>
//
//class Timer {
//    using clock_t = std::chrono::high_resolution_clock;
//    using microseconds = std::chrono::microseconds;
//public:
//    Timer() : start_(clock_t::now()) {}
//
//    ~Timer() = default;
//
//    void print(const std::string &s, int count) {
//        const auto finish = clock_t::now();
//        const auto us =
//                std::chrono::duration_cast<microseconds>
//                        (finish - start_).count();
//        std::cout << s << ": " << us / count << " us" << std::endl;
//    }
//
//    void print(const std::string &s) {
//        print(s, 1);
//    }
//
//private:
//    const clock_t::time_point start_;
//};
//int main(){
//    long a,b,m,x;
//    a=588452287; b=955393174; m=955393177;
//    Timer t2;
//    for (int i = 0; i < 100; ++i) {
//        FindRootMod(a+1,b,m);
//    }
//    t2.print("FindRootMod");
//    Timer t1;
//}