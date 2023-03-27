#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <regex>

#include "../syntactic-analyzer.h"
#include "../utils/eat.cpp"
#include "../utils/verify-productions.cpp"
#include "../../bnf/operators.h"

bool hasParameters(vector<Token> tokens, int *currentToken)
{
    if (verify_content(tokens, currentToken, "("))
    {
        if (identifier(tokens, currentToken))
        {
            if (verify_content(tokens, currentToken, ")"))
            {
                if (eat(tokens, currentToken))
                    return true;
            }
        }
    }
    return false;
}

bool varRead(vector<Token> tokens, int *currentToken)
{
    if (verify_content(tokens, currentToken, "read"))
    {
        return hasParameters(tokens, currentToken);
    }
    return false;
}

bool varWrite(vector<Token> tokens, int *currentToken)
{
    if (verify_content(tokens, currentToken, "write"))
    {
        return hasParameters(tokens, currentToken);
    }
    return false;
}

bool sentences(vector<Token> tokens, int *currentToken)
{
    return verify_productions(tokens, currentToken, {varRead, varWrite});
}
