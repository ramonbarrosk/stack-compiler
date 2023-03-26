#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <regex>
#include <stdexcept>

#include "../syntactic-analyzer.h"
#include "../utils/eat.cpp"
#include "../utils/verify-productions.cpp"

regex types ("integer|pilha_of_real|pilha_of_integer|real");

bool checkVarDeclaration(vector<Token> tokens, int* currentToken) {
    if (verify_content(tokens, currentToken, "var")) {
        return true;
    }
    // caso  não seja "var", volta 1 posição no vector de tokens
    else *currentToken -= 1;
    return false;
}

bool hasIdAfterVar(vector<Token> tokens, int* currentToken) {
    if (checkVarDeclaration(tokens, currentToken)) {
        return identifier(tokens, currentToken);
    }
    return false;
}

bool hasTypeAfterId(vector<Token> tokens, int* currentToken) {
    if (hasIdAfterVar(tokens, currentToken)) {
        if (verify_content(tokens, currentToken, ":")) {
            if (regex_match(tokens[*currentToken].content, types)) {
                return true;
            }
            else {
                *currentToken -= 1;
                string error = "Esperava-se uma tipagem (integer, pilha_of_real, pilha_of_integer, real) após a declaração da variável ";
                string varError = error.append(tokens[*currentToken-1].content);
                throw std::invalid_argument(varError);
            }
        }
        else {
            *currentToken -= 1;
            string error = "Esperava o caractere : após a variável ";
            string varError = error.append(tokens[*currentToken].content);
            throw std::invalid_argument(varError);
        }
    }
    return false;
}

bool declaration(vector<Token> tokens, int* currentToken) {
    return verify_productions(tokens, currentToken, {hasTypeAfterId});
}