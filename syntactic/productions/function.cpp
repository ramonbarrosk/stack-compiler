#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <regex>
#include <stdexcept>

#include "../syntactic-analyzer.h"
#include "../utils/eat.cpp"
#include "../utils/verify-productions.cpp"

bool functionVerifier(vector<Token> tokens, int *currentToken)
{
    if (!isAFunction(tokens, currentToken))
    {
        return false;
    }
    if (verify_content(tokens, currentToken, "("))
    {

        // função tem parametros (necessário pois irá executar a função 
        // que vai contabilizar os tokens dos parametros)
        if (parameters(tokens, currentToken))
        {
            if (verify_content(tokens, currentToken, ")")) {
                // verifica se há o caractere : após os parênteses
                if (verify_content(tokens, currentToken, ":")) {
                    // verifica se há uma tipagem após o caractere :
                    if (regex_match(tokens[*currentToken].content, regex("integer|pilha_of_real|pilha_of_integer|real"))) {
                        return block(tokens, currentToken);
                    } else {
                        string error = "Esperava-se uma tipagem (integer, pilha_of_real, pilha_of_integer, real) após os parênteses da função ";
                        string funcError = error.append(tokens[*currentToken-2].content);
                        throw std::invalid_argument(funcError);
                    }
                } else {
                    string error = "Esperava o caractere : após os parênteses da função ";
                    string funcError = error.append(tokens[*currentToken-2].content);
                    throw std::invalid_argument(funcError);
                }
            } else {
                string error = "Esperava-se um parêntese fechado após os parâmetros da função ";
                string funcError = error.append(tokens[*currentToken-3].content);
                throw std::invalid_argument(funcError);
            }
        }
        // sem parametros
        else {
            if (verify_content(tokens, currentToken, ")")) {
                // verifica se há o caractere : após os parênteses
                if (verify_content(tokens, currentToken, ":")) {
                    // verifica se há uma tipagem após o caractere :
                    if (regex_match(tokens[*currentToken].content, regex("integer|pilha_of_real|pilha_of_integer|real"))) {
                        return eat(tokens, currentToken);
                    } else {
                        string error = "Esperava-se uma tipagem (integer, pilha_of_real, pilha_of_integer, real) após os parênteses da função ";
                        string funcError = error.append(tokens[*currentToken-2].content);
                        throw std::invalid_argument(funcError);
                    }
                } else {
                    string error = "Esperava o caractere : após os parênteses da função ";
                    string funcError = error.append(tokens[*currentToken-2].content);
                    throw std::invalid_argument(funcError);
                }
            } else {
                string error = "Esperava-se um parêntese fechado após os parâmetros da função ";
                string funcError = error.append(tokens[*currentToken-2].content);
                throw std::invalid_argument(funcError);
            }
        }
    }
    return false;
}

bool isAFunction(vector<Token> tokens, int *currentToken)
{
    if (verify_content(tokens, currentToken, "function"))
    {
        if (identifier(tokens, currentToken))
        {
            return true;
        }
    }
    return false;
}

bool bnffunction(vector<Token> tokens, int *currentToken)
{
    return verify_productions(tokens, currentToken, {functionVerifier});
}
