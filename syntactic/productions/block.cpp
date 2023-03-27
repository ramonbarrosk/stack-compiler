#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <regex>

#include "../syntactic-analyzer.h"
#include "../utils/eat.cpp"
#include "../utils/verify-productions.cpp"

bool block(vector<Token> tokens, int *currentToken)
{
    while (true)
    {
        if (!verify_content(tokens, currentToken, "end"))
        {
            verify_productions(tokens, currentToken, {operation, sentences, declaration});
        }
        else
        {
            // Se a verificação falhar, é necessário interromper a análise
            return true;
        }
    }
    // Se o fim do arquivo for alcançado sem encontrar um "end", há um erro de sintaxe
    throw std::invalid_argument("Fim de arquivo inesperado. Esperava-se um \"end\".");
}