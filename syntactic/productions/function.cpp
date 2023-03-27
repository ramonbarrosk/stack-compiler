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
            cout << *currentToken << "\n";
            return (verify_content(tokens, currentToken, ")"));
        }
        return (verify_content(tokens, currentToken, ")"));
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
