#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <regex>
#include <stdexcept>

#include "../syntactic-analyzer.h"
#include "../utils/eat.cpp"
#include "../utils/verify-productions.cpp"

bool procedureVerifier(vector<Token> tokens, int *currentToken)
{
    if (!isAProcedure(tokens, currentToken))
    {
        return false;
    }
    // procedimento tem parametros (necessário pois irá executar a função
    // que vai contabilizar os tokens dos parametros)
    if (verify_content(tokens, currentToken, "("))
    {
        if (parameters(tokens, currentToken))
        {
            if (verify_content(tokens, currentToken, ")"))
            {
                return block(tokens, currentToken);
            }
        }
        // sem parametros
        else
        {
            return block(tokens, currentToken);
        }
    }
    return false;
}

bool isAProcedure(vector<Token> tokens, int *currentToken)
{
    cout << "oi";
    cout << tokens[*currentToken].content  << "\n";
    if (verify_content(tokens, currentToken, "procedure"))
    {
        if (identifier(tokens, currentToken))
        {
            return true;
        }
    }
    return false;
}

bool bnfProcedure(vector<Token> tokens, int *currentToken)
{
    return verify_productions(tokens, currentToken, {procedureVerifier});
}