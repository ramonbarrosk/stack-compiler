#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <regex>

#include "../syntactic-analyzer.h"
#include "../utils/eat.cpp"
#include "../utils/verify-productions.cpp"

bool procedureParameter(vector<Token> tokens, int *currentToken)
{
    int initialToken = *currentToken;
    if (regex_match(tokens[*currentToken].content, types))
    {
        if (identifier(tokens, currentToken))
        {
            return true;
        }
    }
    else if (identifier(tokens, currentToken))
    {
        return true; // parâmetro sem tipo
    }
    *currentToken = initialToken;
    return false;
}

bool procedureParametersProduction(vector<Token> tokens, int *currentToken)
{
    return verify_productions(tokens, currentToken, {procedureParameter});
}


bool callProcedureProduction(vector<Token> tokens, int *currentToken)
{
    int initialToken = *currentToken;
    if (identifier(tokens, currentToken))
    {
        if (verify_content(tokens, currentToken, "("))
        {
            if (parameters(tokens, currentToken))
            {
                if (verify_content(tokens, currentToken, ")"))
                {
                    return true;
                }
            }
            else {
                if (verify_content(tokens, currentToken, ")")) {
                    return true;
                }
            }
        }
        return true;
        // erro na chamada de procedimento
        *currentToken = initialToken;
        return false;
    }

    return false;
}

bool callProcedure(vector<Token> tokens, int *currentToken)
{
    return verify_productions(tokens, currentToken, {callProcedureProduction});
}