#ifndef TASK10_COMPUTER_H
#define TASK10_COMPUTER_H

#include <string>
#include <unordered_map>
#include "Parser.h"

/// вычисляет значение переменных по дереву
class Computer {
public:
    Computer() = default;

    ~Computer() = default;

    /// вычислить все по дереву
    void calc(Node *root);

    /// были ли ошибки
    bool checkError();

    /// текст ошибки
    std::string errorMessage();

private:
    // ограничение на длину выполнения цикла while
    static const int MAX_WHILE_COUNT = 30;
    // карта переменных
    std::unordered_map<std::string, int> vars;
    // сообщение об ошибке
    std::string error_message;
    // факт наличия ошибки
    bool error = false;

    // установить текст ошибки
    void setError(const std::string &s);

    // выполнить 1 операцию над двумя целыми числами
    int computeOper(int first, NodeType oper, int second);

    // выполнить поддерево операций
    int calc(Node *root, int state);

    // заигнорим
    Computer(const Computer &) = delete;

    Computer(const Computer &&) = delete;

    Computer &operator=(const Computer &) const = delete;

    Computer &&operator=(const Computer &&) = delete;
};

#endif //TASK10_COMPUTER_H
