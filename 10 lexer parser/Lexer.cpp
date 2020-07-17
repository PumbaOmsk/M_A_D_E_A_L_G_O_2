// Введенский Вячеслав, MADE-12
//

#include <iostream>
#include "Lexer.h"

bool isSpace(char c);

using namespace std;

bool isLetter(char c) {
    return 'a' <= c and c <= 'z';
}

void Lexer::add(const std::string &line) {
    lines.push_back(line);
}

Token Lexer::next() {
    if (idx_line >= lines.size()) {
        return {TOKEN_EOF, 0, ""};
    }
    while (!eof()) {
        switch (state) {
            case READ_SPACE:
                skipSpace();
                state = checkState();
                break;
            case READ_NAME: {
                string name = readString();
                state = READ_SPACE;
                if (name == "if") {
                    return {TOKEN_IF, 0, name};
                }
                if (name == "else") {
                    return {TOKEN_ELSE, 0, name};
                }
                if (name == "while") {
                    return {TOKEN_WHILE, 0, name};
                }
                return {TOKEN_NAME, 0, name};
            }
            case READ_VALUE: {
                int value = readInt();
                if (state != READ_ERROR) {
                    state = READ_SPACE;
                    return {TOKEN_VALUE, value, ""};
                }
            }
            case READ_OPER: {
                string oper = readOper();
                if (state != READ_ERROR) {
                    state = READ_SPACE;
                    return {TOKEN_OPER, 0, oper};
                }
            }
            case READ_EOL:
                shift();
                skipSpace();
                state = READ_SPACE;
                return {TOKEN_EOL, 0, ""};
            case READ_EOF:
                return {TOKEN_EOF, 0, ""};
            case READ_ERROR:
                return {TOKEN_ERROR, 0, errorMessage};
        }
    }

    return {TOKEN_EOF, 0, ""};
}

TokenType Lexer::tokenType() {
    return current_token.type;
}

string Lexer::oper() {
    return current_token.str;
}

string Lexer::tokenSValue() {
    return current_token.str;
}

int Lexer::tokenValue() {
    return current_token.value;
}

bool Lexer::isClosingBrace() {
    return tokenType() == TOKEN_OPER && oper() == "}";
}

void Lexer::skipEol() {
    while (current_token.type == TOKEN_EOL) {
        shiftToken();
    }
}

void Lexer::shiftToken() {
    prev_idx = idx;
    prev_idx_line = idx_line;
    current_token = next();
}

void Lexer::skipSpace() {
    while (!eof() && isSpace(current()))
        shift();
}

bool isSpace(char c) {
    return c == ' ' || c == '\t';//|| c == '\n';
}

int Lexer::readInt() {
    auto line = lines[idx_line];
    long long result = 0;
    while (idx < line.size() && isdigit(line[idx])) {
        result *= 10;
        result += (int) (line[idx] - '0');
        idx++;
        if (result >= MAX_VALUE) {
            errorMessage = "value is too big: " + to_string(result);
            state = READ_ERROR;
            return 0;
        }
    }
    return (int) result;
}

string Lexer::readString() {
    unsigned int b = idx;
    auto line = lines[idx_line];
    while (idx < line.size() && isLetter(line[idx])) {
        idx++;
    }
    return line.substr(b, idx - b);
}

string Lexer::readOper() {
    auto c = current();
    shift();
    return std::string(1, c);
}

void Lexer::shift() {
    if (eof())
        return;
    if (idx < lines[idx_line].size()) {
        idx++;
        return;
    }
    idx = 0;
    idx_line++;
}

bool Lexer::eof() {
    return idx_line >= lines.size();
}

bool Lexer::eol() {
    if (idx_line >= lines.size())
        return true;
    return idx >= lines[idx_line].size();
}

char Lexer::current() {
    if (eof())
        return 0;
    if (eol())
        return '\n';
    return lines[idx_line][idx];
}

ReadState Lexer::checkState() {
    if (eof())
        return READ_EOF;
    char c = current();
    switch (c) {
        case 0:
            return READ_EOF;
        case '\n':
            return READ_EOL;
        case '\t':
        case ' ':
            return READ_SPACE;
        case '=':
        case '+':
        case '-':
        case '*':
        case '/':
        case '(':
        case ')':
        case '{':
        case '}':
            return READ_OPER;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            return READ_VALUE;
        default:
            if (isLetter(c))
                return READ_NAME;
            errorMessage = "Error token at pos " + to_string(idx_line) + "," + to_string(idx);
    }
    return READ_ERROR;
}

bool Lexer::isError() {
    return state == READ_ERROR;
}

void Lexer::clear() {
    lines.clear();
    state = READ_SPACE;
    errorMessage = "";
    idx = 0;
    idx_line = 0;
}

void Lexer::printSource() {
    cout << "-- begin source" << endl;
    for (const auto &line:lines) {
        cout << line << endl;
    }
    cout << "-- end source" << endl;
}

string Lexer::lastPosition() {
    return to_string(prev_idx_line) + ":" + to_string(prev_idx);
}

string toStringTokenType(TokenType type) {
    switch (type) {
        case TOKEN_EOF:
            return "TOKEN_EOF";
        case TOKEN_EOL:
            return "TOKEN_EOL";
        case TOKEN_NAME:
            return "TOKEN_NAME";
        case TOKEN_VALUE:
            return "TOKEN_VALUE";
        case TOKEN_OPER:
            return "TOKEN_OPER";
        case TOKEN_ERROR:
            return "TOKEN_ERROR";
        case TOKEN_IF:
            return "TOKEN_IF";
        case TOKEN_ELSE:
            return "TOKEN_ELSE";
        case TOKEN_WHILE:
            return "TOKEN_WHILE";
    }
    return "unknown";
}

string Token::toString() {
    return "(" + toStringTokenType(type) + "," + to_string(value) + "," + str + ")";
}
