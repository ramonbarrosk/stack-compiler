#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <regex>
#include <stdexcept>

#include "../syntactic-analyzer.h"
#include "../utils/eat.cpp"
#include "../utils/verify-productions.cpp"

string error;
string funcError;

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
                if (verify_content(tokens, currentToken, ";"))
                {
                    if (block(tokens, currentToken))
                    {
                        if (verify_content(tokens, currentToken, ";"))
                        {
                            return true;
                        }

                        else
                        {
                            error = "Bloco do procedimento mal escrito";
                            funcError = error.append(tokens[*currentToken - 2].content);
                            throw std::invalid_argument(funcError);
                        }
                    }
                }
            }
        }
        // sem parametros
        else
        {
            if (block(tokens, currentToken))
            {
                if (verify_content(tokens, currentToken, ";"))
                {
                    return true;
                }
            }
            else
            {
                error = "Erro de bloco";
                funcError = error.append(tokens[*currentToken - 2].content);
                throw std::invalid_argument(funcError);
            }
        }
    }
    return false;
}

bool isAProcedure(vector<Token> tokens, int *currentToken)
{
    if (verify_content(tokens, currentToken, "procedure"))
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
    return verify_productions(tokens, currentToken, {procedureVerifier});
}