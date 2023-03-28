#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <regex>
#include <stdexcept>

#include "../syntactic-analyzer.h"
#include "../utils/eat.cpp"
#include "../utils/verify-productions.cpp"

bool termo(vector<Token> tokens, int* currentToken) {
    return verify_productions(tokens, currentToken, {operation, identifier}) || tokens[*currentToken].type.compare("IDENTIFIER") == 0;
}

bool expressao_num(vector<Token> tokens, int *currentToken) {
    if (termo(tokens, currentToken)) {
        return true;
    } else if (identifier(tokens, currentToken)) {
        if (argumentos(tokens, currentToken)) {
            return true;
        }
        return false;
    }
    return false;
}

bool expr(vector<Token> tokens, int *currentToken) {
    return verify_productions(tokens, currentToken, {expressao_num});
}