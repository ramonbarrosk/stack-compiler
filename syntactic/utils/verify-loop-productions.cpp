#pragma once
#include "../syntactic-analyzer.h"

// mesma ideia do verify_productions, mas o body roda em loop pra pegar o input todo ao invés de parar na primeira produção q soltar um true
bool verify_loop_productions(vector<Token> tokens, int *currentToken, vector<bool (*)(vector<Token>, int *)> productions)
{
    int initialToken = *currentToken;
    for (auto &f : productions)
    {
        for (int i = 0; i < 10; i++)
        { // Loop interno
            f(tokens, currentToken);
        }
    }
    return true; // retorna verdadeiro se todas as produções foram encontradas
}