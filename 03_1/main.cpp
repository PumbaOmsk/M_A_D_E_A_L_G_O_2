/*
 * Введенский Вячеслав В. MADE-ML-22
 *
A. Массовое разложение на множители
Дано много чисел. Требуется разложить их все на простые множители.
Входные данные
В первой строке задано число n (2 ≤ n ≤ 300000). В следующих n строках заданы числа ai (2 ≤ ai ≤ 1e6),
 которые нужно разложить на множители.
Выходные данные
Для каждого числа выведите в отдельной строке разложение на простые множители в порядке возрастания множителей. */

//#define _DEBUG_
#include <iostream>
#include <cstdio>
#include <list>
#include <vector>

#ifdef _DEBUG_
#include <random>
#endif

using namespace std;

/**
 * Максимальное значение входных данных.
 */
const int MAX_VALUE = 1000001;

/**
 * Разложение числа на простые множители.
 * @param v число.
 * @param min_dev вектор минимальных делителей для числа.
 * @return список простых множителей в порядке неубывания.
 */
list<int> CountPrimeFactors(int v, const vector<int> &min_dev) {
    list<int> factors;
    while (min_dev[v] != v) {
        factors.push_back(min_dev[v]);
        v = v / min_dev[v];
    }
    if (v != 1)
        factors.push_back(v);
    return factors;
}

/**
 * Быстрое чтение положительного целого числа из консоли.
 * @param out куда положить результат
  */
bool read_int_positive(int &out) {
    int c = getchar();
    int x = 0;
    for (; !('0' <= c && c <= '9') && c != '-'; c = getchar()) {
        if (c == EOF)
            return false;
    }
    if (c == EOF)
        return false;
    for (; '0' <= c && c <= '9'; c = getchar()) {
        x = x * 10 + c - '0';
    }
    out = x;
    return true;
}

int main() {
    int n;
#ifndef _DEBUG_
    read_int_positive(n);
#else
    n=300000;
    std::uniform_int_distribution<uint32_t> rnd(2, MAX_VALUE);
    std::default_random_engine re(17);
#endif
    vector<int> min_dev(MAX_VALUE, 0);
    list<int> primes;
    // заполним вектор минимальных делителей для чисел.
    for (int i = 2; i < MAX_VALUE; i++) {
        if (min_dev[i] == 0) {
            min_dev[i] = i;
            primes.push_back(i);
        }
        for (int p : primes) {
            if (p > min_dev[i] || i * p >= MAX_VALUE) {
                break;
            }
            min_dev[i * p] = p;
        }
    }

    for (int i = 0; i < n; ++i) {
        int v;
#ifndef _DEBUG_
        read_int_positive(v);
#else
        v = rnd(re);
#endif
        auto factors = CountPrimeFactors(v, min_dev);
        for (int p:factors) {
            printf("%i ", p);
        }
        printf("\n");
    }
    cout << flush;
    return 0;
}