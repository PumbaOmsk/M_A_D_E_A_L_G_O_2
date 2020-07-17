#ifndef TASK10_PARSER_H
#define TASK10_PARSER_H

#include <string>
#include <vector>
#include "Lexer.h"

/// тип вершины в дереве
enum NodeType {
    NODE_PROG,
    NODE_VAR_ASSIGN,
    NODE_VAR,
    NODE_EXPR,
    NODE_ADD_OPER,
    NODE_ADD,
    NODE_SUB,
    NODE_EXPR_MUL,
    NODE_MUL_OPER,
    NODE_MUL,
    NODE_DIV,
    NODE_PARENTHESES,
    NODE_IF,
    NODE_WHILE,
    NODE_BLOCK,
    NODE_VALUE
};

/// вершина
struct Node {
    Node(NodeType _type, std::string _s_value, int _value) : type(_type), s_value(std::move(_s_value)), value(_value) {}

    ~Node() { // не забываем все удалить за собой
        for (auto child : childs) {
            delete child;
        }
    }

    /// добавить ветку к вершине
    void add(Node *_child) {
        if (_child != nullptr)
            childs.push_back(_child);
    }

    /// тип вершины
    NodeType type;
    /// строковое значение в вершине
    std::string s_value;
    /// целочисленное значение в вершине
    int value;
    /// вектор детей - веток
    std::vector<Node *> childs;
};

/// парсер грамматики
class Parser {
public:
    /// для работы нужен Lexer
    explicit Parser(Lexer &_lexer) : lexer(_lexer) {}

    ~Parser() = default;

    /// выполнить разбор всего
    Node *parse();

    /// есть ли ошибки
    bool checkError();

    /// текст ошибки
    std::string errorMessage();

private:
    // лексер
    Lexer &lexer;
    // текст ошибки
    std::string error_message;
    // факт наличия ошибки
    bool error = false;

    // запрещаем копирование класса
    Parser(const Parser &parser) = delete;

    Parser(const Parser &&parser) = delete;

    Parser &operator=(const Parser &parser) const = delete;

    Parser &&operator=(const Parser &&parser) = delete;

    void setError(const std::string &s);

    /// всей программы
    Node *parseProg();

    /// блока из нескольких строк (также для if/else/while).
    Node *parseBlock();

    /// одной строки a=...
    Node *parseState();

    /// цикла while
    Node *parseWhile();

    /// if/else
    Node *parseIf();

    /// условия для if/while
//    Node *parseCondition();  - оказалось, что можно использовать parseParentheses

    /// первой части для if (для while тоже она)
    Node *parseIfTrue();

    /// второй части для if
    Node *parseIfFalse();

    /// имени переменной
    Node *parseVar();

    /// выражения
    Node *parseExpr();

    /// +- PS
    Node *parseAdd();

    /// PS
    Node *parseMult();

    /// */PS
    Node *parseMult2();

    /// базовые части выражений: число, переменная, скобки ()
    Node *parseBase();

    /// круглые скобки
    Node *parseParentheses();
};

#endif //TASK10_PARSER_H
