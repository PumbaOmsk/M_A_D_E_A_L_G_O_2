#include <iostream>
#include "Computer.h"

using namespace std;

void Computer::calc(Node *root) {
    vars.clear();
    calc(root, 0);
}

bool Computer::checkError() {
    return error;
}

string Computer::errorMessage() {
    return error_message;
}

void Computer::setError(const string &s) {
    error_message = s;
    error = true;
}

int Computer::computeOper(int first, NodeType oper, int second) {
    switch (oper) {
        case NODE_ADD:
            return first + second;
        case NODE_SUB:
            return first - second;
        case NODE_MUL:
            return first * second;
        case NODE_DIV:
            if (second == 0) {
                setError("devide by zero");
                return 0;
            }
            return first / second;
        default:// error
            return 0;
    }
}

int Computer::calc(Node *root, int state) {
    if (root == nullptr)
        return state;
    if (checkError())
        return 0;
    if (root->type == NODE_MUL_OPER || root->type == NODE_ADD_OPER) {
        int second = calc(root->childs[1], 0);
        state = computeOper(state, root->childs[0]->type, second);
        for (int i = 2; i < root->childs.size(); ++i) {
            state = calc(root->childs[i], state);
        }
        return state;
    }
    if (root->type == NODE_VAR_ASSIGN) {
        auto var_name = root->childs[0]->s_value;
        state = calc(root->childs[1], 0);
        vars[var_name] = state;
        if (checkError()) {
            cout << var_name << " = " << errorMessage() << endl;
        } else {
            cout << var_name << " = " << state << endl;
        }
        return state;
    }
    if (root->type == NODE_VAR) {
        auto var_name = root->s_value;
        auto it = vars.find(var_name);
        if (it == vars.end()) {
            setError("unknown variable: " + var_name);
            return 0;
        }
        return it->second;
    }
    if (root->type == NODE_IF) {
        int first = calc(root->childs[0], 0);
        if (first != 0) {
            return calc(root->childs[1], 0);
        } else {
            if (root->childs.size() > 2) {
                return calc(root->childs[2], 0);
            }
        }
    }
    if (root->type == NODE_WHILE) {
        int count = 0;
        while (calc(root->childs[0], 0) != 0) {
            calc(root->childs[1], 0);
            count++;
            if (count > MAX_WHILE_COUNT) {
                setError("while cycle greater than " + to_string(MAX_WHILE_COUNT));
                return 0;
            }
        }
        return 0;
    }
    if (!root->childs.empty()) {
        for (auto &child : root->childs) {
            state = calc(child, state);
        }
        return state;
    }
    switch (root->type) {
        case NODE_PROG:
            return 0;
        case NODE_EXPR:
            break;
        case NODE_BLOCK:
            break;
        case NODE_ADD:
            break;
        case NODE_SUB:
            break;
        case NODE_EXPR_MUL:
            break;
        case NODE_MUL:
            break;
        case NODE_DIV:
            break;
        case NODE_PARENTHESES:
            break;
        case NODE_VALUE:
            return root->value;
    }
    cout << "Ooooops!" << endl;
    return 0;
}