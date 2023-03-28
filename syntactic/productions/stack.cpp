#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <regex>
#include <stdexcept>

#include "../syntactic-analyzer.h"
#include "../utils/eat.cpp"
#include "../utils/verify-productions.cpp"

bool integerNumCont(vector<Token> tokens, int *currentToken)
{
    if (tokens[*currentToken].type.compare("NUMBER") == 0)
    {
        eat(tokens, currentToken);
        if (verify_content(tokens, currentToken, ","))
        {
            if (tokens[*currentToken].type.compare(DELIMITER) == 0)
            {
                error = "Delimitador inválido.";
                throw std::invalid_argument(error);
            }
            return integerNumCont(tokens, currentToken);
        }
        else
        {
            if (verify_content(tokens, currentToken, "#"))
            {
                return true;
            }
            else
            {
                error = "Era esperado o delimitador # para fechar a operação de pilha.";
                throw std::invalid_argument(error);
            }
        }
    }
    else
    {
        if (verify_content(tokens, currentToken, "#"))
        {
            return true;
        }
    }
    return true; // a parte fracionária é opcional
}

bool conteudo(vector<Token> tokens, int *currentToken)
{
    if (verify_content(tokens, currentToken, "#"))
    {
        if (verify_content(tokens, currentToken, "#"))
        {
            return true;
        }
        else
        {
            return integerNumCont(tokens, currentToken);
        }
    }
    return false;
}

bool expressaoPilha(vector<Token> tokens, int *currentToken)
{
    if (verify_content(tokens, currentToken, "inverte") || verify_content(tokens, currentToken, "input") || verify_content(tokens, currentToken, "output") || verify_content(tokens, currentToken, "length"))
    {
        if (verify_content(tokens, currentToken, "("))
        {

            if (conteudo(tokens, currentToken))
            {
                if (verify_content(tokens, currentToken, ")"))
                {
                    return true;
                }
                else
                {
                    error = "Era esperado um )";
                    throw std::invalid_argument(error);
                }
            }
            else
            {
                error = "Era esperado um conteúdo válido";
                throw std::invalid_argument(error);
            }
        }
    }
    else if (verify_content(tokens, currentToken, "concatena"))
    {
        if (verify_content(tokens, currentToken, "("))
        {
            if (conteudo(tokens, currentToken))
            {

                if (verify_content(tokens, currentToken, ","))
                {
                    if (conteudo(tokens, currentToken))
                    {
                        if (verify_content(tokens, currentToken, ")"))
                        {
                            return true;
                        }
                        else
                        {
                            error = "Era esperado um )";
                            throw std::invalid_argument(error);
                        }
                    }
                    error = "Era esperado  uma outra operação de pilha";
                    throw std::invalid_argument(error);
                }
                else {
                    error = "O operador \"concatena\" deve receber dois parametros.";
                    throw std::invalid_argument(error);
                }
            }
            else
            {
                error = "Era esperado um conteúdo válido";
                throw std::invalid_argument(error);
            }
        }
    }
    return false;
}

bool stackProduction(vector<Token> tokens, int *currentToken)
{
    return verify_productions(tokens, currentToken, {expressaoPilha});
}