#include "Parser.h"

using namespace std;

Node *Parser::parse() {
    return parseProg();
}

bool Parser::checkError() {
    return error;
}

std::string Parser::errorMessage() {
    return error_message;
}


Node *Parser::parseProg() {
    lexer.shiftToken();
    Node *_prog = new Node(NODE_PROG, "", 0);
    Node *_state = parseBlock();
    if (checkError())
        return _prog;
    lexer.shiftToken();
    _prog->add(_state);
    return _prog;
}

Node *Parser::parseBlock() {
    Node *_block = new Node(NODE_BLOCK, "", 0);
    while (lexer.tokenType() != TOKEN_EOF && !lexer.isClosingBrace()) {
        Node *_state = parseState();
        if (checkError())
            return _block;
        lexer.skipEol();
        _block->add(_state);
    }
    return _block;
}

Node *Parser::parseState() {
    lexer.skipEol();
    if (lexer.tokenType() == TOKEN_NAME) {
        Node *token_var = parseVar();
        if (lexer.tokenType() == TOKEN_OPER && lexer.oper() == "=") {
            lexer.shiftToken();
            Node *expr = parseExpr();
            auto state = new Node(NODE_VAR_ASSIGN, "", 0);
            state->add(token_var);
            state->add(expr);
            return state;
        }
    } else if (lexer.tokenType() == TOKEN_IF) {
        lexer.shiftToken();
        Node *token_if = parseIf();
        return token_if;
    } else if (lexer.tokenType() == TOKEN_WHILE) {
        lexer.shiftToken();
        Node *token_while = parseWhile();
        return token_while;
    }
    setError("error token");
    return nullptr;
}

Node *Parser::parseWhile() {
    Node *_condition = parseParentheses();
//    Node *_condition = parseCondition();
    Node *_cycle = parseIfTrue();
    if (lexer.isError()) {
        return nullptr;
    }
    auto _current = new Node(NODE_WHILE, "", 0);
    _current->add(_condition);
    _current->add(_cycle);
    return _current;
}

Node *Parser::parseIf() {
    Node *_condition = parseParentheses();
//    Node *_condition = parseCondition();
    Node *_if_true = parseIfTrue();
    Node *_if_false = parseIfFalse();
    if (lexer.isError()) {
        return nullptr;
    }
    auto _current = new Node(NODE_IF, "", 0);
    _current->add(_condition);
    _current->add(_if_true);
    _current->add(_if_false);
    return _current;
}

//Node *Parser::parseCondition() {
//    if (lexer.tokenType() == TOKEN_OPER && lexer.oper() == "(") {
//        lexer.shiftToken();
//        Node *_parentheses = parseParentheses();
//        return _parentheses;
//    }
//    setError("error conditon structure");
//    return nullptr;
//}

Node *Parser::parseIfTrue() {
    if (lexer.tokenType() == TOKEN_OPER && lexer.oper() == "{") {
        lexer.shiftToken();
        lexer.skipEol();
        Node *_block = parseBlock();
        lexer.skipEol();
        if (lexer.tokenType() == TOKEN_OPER && lexer.oper() == "}") {
            lexer.shiftToken();
            lexer.skipEol();
            return _block;
        }
    }
    setError("error TRUE structure");
    return nullptr;
}

Node *Parser::parseIfFalse() {
    if (lexer.tokenType() == TOKEN_ELSE) {
        lexer.shiftToken();
        lexer.skipEol();
        if (lexer.tokenType() == TOKEN_OPER && lexer.oper() == "{") {
            lexer.shiftToken();
            Node *_block = parseBlock();
            lexer.skipEol();
            if (lexer.tokenType() == TOKEN_OPER && lexer.oper() == "}") {
                lexer.shiftToken();
                return _block;
            }
        }
    }
    return nullptr;
}

void Parser::setError(const string &s) {
    if (!error) {
        error_message = s + " at pos " + lexer.lastPosition();
        error = true;
    }
}

Node *Parser::parseVar() {
    if (lexer.tokenType() == TOKEN_NAME) {
        auto _current = new Node(NODE_VAR, lexer.tokenSValue(), 0);
        lexer.shiftToken();
        return _current;
    }
    setError("variable name expected");
    return nullptr;
}

Node *Parser::parseExpr() {
    Node *token_mult = parseMult();
    Node *token_add = parseAdd();
    Node *token_expr = new Node(NODE_EXPR, "", 0);
    token_expr->add(token_mult);
    token_expr->add(token_add);
    return token_expr;
}

Node *Parser::parseAdd() {
    auto _oper_name = lexer.oper();
    if (lexer.tokenType() == TOKEN_OPER && (_oper_name == "+" || _oper_name == "-")) {
        lexer.shiftToken();
        Node *_oper = nullptr;
        if (_oper_name == "+")
            _oper = new Node(NODE_ADD, "", 0);
        else
            _oper = new Node(NODE_SUB, "", 0);
        Node *_mult = parseMult();
        Node *_add = parseAdd();

        Node *_current = new Node(NODE_ADD_OPER, "", 0);
        _current->add(_oper);
        _current->add(_mult);
        _current->add(_add);

        return _current;
    }
    if (lexer.tokenType() != TOKEN_EOL && lexer.tokenType() != TOKEN_EOF && lexer.tokenType() != TOKEN_OPER) {
        setError("invalid structure");
    }
    return nullptr;
}

Node *Parser::parseMult() {
    Node *_base = parseBase();
    Node *_mult = parseMult2();
    Node *_current = new Node(NODE_EXPR_MUL, "", 0);
    _current->add(_base);
    _current->add(_mult);
    return _current;
}

Node *Parser::parseMult2() {
    auto _oper_name = lexer.oper();
    if (lexer.tokenType() == TOKEN_OPER && (_oper_name == "*" || _oper_name == "/")) {
        lexer.shiftToken();
        Node *_oper = nullptr;
        if (_oper_name == "*")
            _oper = new Node(NODE_MUL, "", 0);
        else
            _oper = new Node(NODE_DIV, "", 0);

        Node *_base = parseBase();
        Node *_mult = parseMult2();
        Node *_current = new Node(NODE_MUL_OPER, "", 0);
        _current->add(_oper);
        _current->add(_base);
        _current->add(_mult);
        return _current;
    }
    if (lexer.tokenType() != TOKEN_EOL && lexer.tokenType() != TOKEN_EOF && lexer.tokenType() != TOKEN_OPER) {
        setError("invalid structure");
    }
    return nullptr;
}

Node *Parser::parseBase() {
    Node *_current = nullptr;
    switch (lexer.tokenType()) {
        case TOKEN_VALUE:
            _current = new Node(NODE_VALUE, "", lexer.tokenValue());
            break;
        case TOKEN_NAME:
            _current = new Node(NODE_VAR, lexer.tokenSValue(), 0);
            break;
        case TOKEN_OPER:
            if (lexer.oper() == "(") {
//                lexer.shiftToken();
                return parseParentheses();
            }
        default:
            setError("invalid string structure");
            return nullptr;
    }
    lexer.shiftToken();
    return _current;
}

Node *Parser::parseParentheses() {
    if (lexer.tokenType() == TOKEN_OPER && lexer.oper() == "(") {
        lexer.shiftToken();
        auto _expr = parseExpr();
        if (lexer.tokenType() == TOKEN_OPER && lexer.oper() == ")") {
            lexer.shiftToken();
            auto _current = new Node(NODE_PARENTHESES, "", 0);
            _current->add(_expr);
            return _current;
        }
    }
    return nullptr;
}