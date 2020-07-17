/**
 * Введенский Вячеслав, MADE-22
Ввод:
a = 2
b = 3
c = a+b+1
d = c-2
вывод:
a = 2
b = 3
c = 6
d = 4
Поддерживаемые операции:
*,/,+,-,(,),if/else,while
Перезапись переменных,Обработка ошибок, Тесты
Дополнительные расширения
 // скобки обязательны
 if(a-b){
   a=(a-b)*c
 }else{   // можно опускать.
   a=(a+b)*c
 }

 while(a){
    c=c+1
    a=a-1
 }
 */
#include <utility>
#include <iostream>
#include <list>
#include <set>
#include <fstream>
#include "Lexer.h"
#include "Parser.h"
#include "Computer.h"

using namespace std;

/// для печати дерева
void print(Node *node) {
    if (node == nullptr) {
        return;
    }
    switch (node->type) {
        case NODE_PROG:
            cout << "PROG";
            break;
        case NODE_VAR_ASSIGN:
            cout << "=";
            break;
        case NODE_VAR:
            cout << "VAR " << node->s_value;
            break;
        case NODE_EXPR:
            cout << "EXPR";
            break;
        case NODE_ADD_OPER:
            cout << "NODE_ADD_OPER";
            break;
        case NODE_ADD:
            cout << "ADD";
            break;
        case NODE_SUB:
            cout << "SUB";
            break;
        case NODE_EXPR_MUL:
            cout << "EXPR_MUL";
            break;
        case NODE_MUL_OPER:
            cout << "NODE_MUL_OPER";
            break;
        case NODE_MUL:
            cout << "MUL";
            break;
        case NODE_DIV:
            cout << "DIV";
            break;
        case NODE_PARENTHESES:
            cout << "()";
            break;
        case NODE_VALUE:
            cout << node->value;
            break;
        case NODE_IF:
            cout << "IF";
            break;
        case NODE_WHILE:
            cout << "WHILE";
            break;
        case NODE_BLOCK:
            cout << "BLOCK";
            break;
    }
}

/// печать на экран
void print(Node *node, int deep) {
    if (node == nullptr) {
        return;
    }
    for (int i = 0; i < deep; ++i) {
        cout << "  ";
    }
    print(node);
    cout << endl;
    for (auto child:node->childs) {
        print(child, deep + 1);
    }
}


/**
 * Вывод краткой подсказки.
 */
void printHelp() {
    cout << "calc some expression" << endl;
    cout << "usage:" << endl;
    cout << "calc <filename>" << endl;
    cout << "or" << endl;
    cout << "calc - interactive mode" << endl;
    cout << "type a few expression and type :c to calculate" << endl;
    cout << "can use \":----\" line to separate blocks" << endl;
    cout << ">" << endl;
}

/// загрузить из файла
vector<string> loadFromFile(char *fn) {
    vector<string> lines;
    ifstream fin(fn);
    string line;
    while (getline(fin, line)) {
        lines.push_back(line);
    }
    return lines;
}

/// загрузить из потока
vector<string> loadFromCin() {
    vector<string> lines;
    string line;
    while (!cin.eof()) {
        cin >> line;
        if (line == ":c")
            break;
        lines.push_back(line);
    }
    return lines;
}

/// посчитать один блок.
void calcBlock(Lexer &lexer) {
    cout << ":--------------------------------------------" << endl;
    lexer.printSource();
    Parser parser(lexer);
    Node *tree = parser.parse();
    if (parser.checkError()) {
        cout << "Parser error: " << parser.errorMessage() << endl;
    } else {
//        print(tree,0);
        Computer computer;
        computer.calc(tree);
        if (computer.checkError()) {
            cout << "Interpretator error: " << computer.errorMessage() << endl;
        }
    }
    delete tree;
    lexer.clear();
}

int main(int argc, char *argv[]) {
    vector<string> lines;
    if (argc > 1) {
        lines = loadFromFile(argv[1]);
    } else {
        printHelp();
        lines = loadFromCin();
    }

    Lexer lexer;
    // кормим лексер блоками
    for (int i = 0; i < lines.size(); ++i) {
        auto line = lines[i];
        if (line.find(":-") != 0) {
            lexer.add(line);
        }
        if (line.find(":-") == 0 || i == lines.size() - 1) {
            calcBlock(lexer);
        }
    }

    return 0;
}
