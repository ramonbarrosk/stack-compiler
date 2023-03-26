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
        // verifica se tem ":" para analisar a tipagem
        if (verify_content(tokens, currentToken, ":")) {
            // verifica se a tipagem é valida
            if (regex_match(tokens[*currentToken].content, types)) {
                return eat(tokens, currentToken);
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

bool hasMoreIds(vector<Token> tokens, int* currentToken) {
    if (!verify_content(tokens, currentToken, ";")) {
        while (true) {
            cout << tokens[*currentToken].content << "\n";
            eat(tokens, currentToken);
            if (verify_content(tokens, currentToken, ";")) {
                return true;
            }
        }
    }
    else return eat(tokens, currentToken);
    return false;
}

bool declaration(vector<Token> tokens, int* currentToken) {
    return verify_productions(tokens, currentToken, {hasMoreIds});
}