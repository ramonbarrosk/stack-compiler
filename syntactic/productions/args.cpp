#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <regex>
#include <stdexcept>

#include "../syntactic-analyzer.h"
#include "../utils/eat.cpp"
#include "../utils/verify-productions.cpp"

bool argumentos(vector<Token> tokens, int *currentToken) {
    if (verify_content(tokens, currentToken, "(")) {
        if (lista_arg(tokens, currentToken)) {
            if (verify_content(tokens, currentToken, ")")) {
                return true;
            }
        }
        return false;
    }
    return true; // <empty>
}

bool lista_arg(vector<Token> tokens, int *currentToken) {
    if (expr(tokens, currentToken)) {
        if (cont_lista_arg(tokens, currentToken)) {
            return true;
        }
        return false;
    }
    return false;
}

bool cont_lista_arg(vector<Token> tokens, int *currentToken) {
    if (verify_content(tokens, currentToken, ",")) {
        if (lista_arg(tokens, currentToken)) {
            return true;
        }
        return false;
    }
    return true; // <empty>
}

bool args(vector<Token> tokens, int *currentToken) {
    return verify_productions(tokens, currentToken, {argumentos});
}