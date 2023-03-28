#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <regex>

#include "../syntactic-analyzer.h"
#include "../utils/eat.cpp"
#include "../utils/verify-productions.cpp"

// mesma ideia do verify_productions, mas o body roda em loop pra pegar o input todo ao invés de parar na primeira produção q soltar um true
bool verify_body_productions(vector<Token> tokens, int *currentToken, vector<bool (*)(vector<Token>, int*)> productions) {
    int initialToken = *currentToken;
    for (auto& f : productions) {
        if (f(tokens, currentToken)) {
            continue;
        } else {
            *currentToken = initialToken; // retorna o token para a posição inicial
            return false; // retorna falso se a produção não foi encontrada
        }
    }
    return true; // retorna verdadeiro se todas as produções foram encontradas
}


bool body(vector<Token> tokens, int* currentToken) {
    // tirei o declaration por enquanto do body, essa merda funciona pra o block, mas pra ca n. é o pijas.
    return verify_body_productions(tokens, currentToken, {bnffunction, isBeginAndEnd});
}
// o return do body vai ficar mais ou menos assim:
// return verify_productions(tokens, currentToken, {declaration, routines, begin&end}); 
