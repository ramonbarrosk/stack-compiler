#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <regex>

#include "../syntactic-analyzer.h"
#include "../utils/eat.cpp"
#include "../utils/verify-productions.cpp"
#include "../../bnf/identifiers.h"
#include "../../bnf/types.h"

regex functionTypes("integer|pilha_of_real|pilha_of_integer|real");

bool cont_lista_id(vector<Token> tokens, int *currentToken)
{
    if (verify_content(tokens, currentToken, ","))
    {
        if (lista_id(tokens, currentToken))
        {
            if (eat(tokens, currentToken))
                return true;
        }
    }
    return true;
}

bool lista_id(vector<Token> tokens, int *currentToken)
{
    if (identifier(tokens, currentToken))
    {
        return cont_lista_id(tokens, currentToken);
    }
    return false;
}

bool lista_parametros(vector<Token> tokens, int *currentToken)
{
    if (lista_id(tokens, currentToken))
    {
        if (verify_content(tokens, currentToken, ":"))
        {
            if (regex_match(tokens[*currentToken].content, functionTypes))
            {
                return cont_lista_parametros(tokens, currentToken);
            }
        }
    }
    return false;
}

bool cont_lista_parametros(vector<Token> tokens, int *currentToken)
{
    if (!eat(tokens, currentToken))
        return false;
    if (verify_content(tokens, currentToken, ";"))
    {
        if (lista_parametros(tokens, currentToken))
        {
            if (!verify_content(tokens, currentToken, ";"))
                return true;
        }
    }
    return true;
}


bool parameters(vector<Token> tokens, int *currentToken)
{
    return verify_productions(tokens, currentToken, {lista_parametros});
}
