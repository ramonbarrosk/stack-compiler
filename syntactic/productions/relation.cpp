#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <regex>

#include "../../bnf/operators.h"
#include "../syntactic-analyzer.h"
#include "../utils/eat.cpp"
#include "./operation.cpp"
#include "../utils/verify-productions.cpp"

#pragma once

using namespace std;
regex relationalOperators("=|>|<|>=|<=|<>");

bool consume_relational_operator(vector<Token> tokens, int *currentToken)
{
    // Verifica se o token atual é um operador válido
    if (regex_match(tokens[*currentToken].content, relationalOperators))
    {
        return eat(tokens, currentToken);
    }
    return false;
}

bool consume_relational_operation(vector<Token> tokens, int *currentToken)
{
    // Verifica se o token atual é um operador válido
    if (consume_relational_operator(tokens, currentToken))
    {
        // Verifica se há um parêntese aberto após o operador
        if (verify_content(tokens, currentToken, "("))
        {
            // Consome o primeiro operando
            if (consume_operand(tokens, currentToken))
            {
                // Verifica se há uma vírgula após o primeiro operando
                if (verify_content(tokens, currentToken, ","))
                {
                    // Consome o segundo operando
                    if (consume_operand(tokens, currentToken))
                    {
                        // Verifica se há um parêntese fechado após o segundo operando
                        if (verify_content(tokens, currentToken, ")"))
                        {
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}


bool relation(vector<Token> tokens, int *currentToken)
{
    return verify_productions(tokens, currentToken, {consume_relational_operation});
}