#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <regex>

#include "../syntactic-analyzer.h"
#include "../utils/eat.cpp"
#include "../utils/verify-productions.cpp"
#include "../../bnf/operators.h"

// esse hasParameters verifica só se tem um identificador dentro dos parenteses. tipo: write(x)
bool hasParameters(vector<Token> tokens, int *currentToken)
{
    if (!verify_content(tokens, currentToken, "("))
    {
        error = "Esperava-se um abre parêntese no início dos parâmetros";
        throw std::invalid_argument(funcError);
    }
    if (!identifier(tokens, currentToken))
    {
        error = "Esperava-se um identificador para o nome do parâmetro";
        throw std::invalid_argument(funcError);
    }
    if (!verify_content(tokens, currentToken, ")"))
    {
        error = "Esperava-se um fecha parêntese no final dos parâmetros";
        throw std::invalid_argument(funcError);
    }
    if (!eat(tokens, currentToken))
    {
        error = "Erro ao consumir token de fechamento de parâmetros";
        throw std::invalid_argument(funcError);
    }
    return true;
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

bool ifSentence(vector<Token> tokens, int *currentToken)
{
    if (verify_content(tokens, currentToken, "if"))
    {
        if (verify_content(tokens, currentToken, "("))
        {
            if (!relation(tokens, currentToken))
            {
                error = "Erro na condição do if";
                throw std::invalid_argument(error);
            }
            if (!verify_content(tokens, currentToken, ")"))
            {
                error = "Esperava-se um fecha parêntese após a condição do if";
                throw std::invalid_argument(error);
            }
            if (!verify_content(tokens, currentToken, "then"))
            {
                error = "Esperava-se a palavra 'then' após a condição do if";
                throw std::invalid_argument(error);
            }
            return block(tokens, currentToken);
        }
        error = "Esperava-se um abre parêntese após o if";
        throw std::invalid_argument(error);
    }
    return false;
}

bool repeatUntilSentence(vector<Token> tokens, int *currentToken)
{
    if (verify_content(tokens, currentToken, "repeat"))
    {
        if (sentences(tokens, currentToken) || callProcedure(tokens, currentToken) || operation(tokens, currentToken))
        {
            if (verify_content(tokens, currentToken, "until"))
            {
                return relation(tokens, currentToken);
            }
        }
        else
        {
            throw std::invalid_argument(error);
        }
    }
    return false;
}

bool forSentence(vector<Token> tokens, int *currentToken)
// gambiarra da peste KKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKKK
{
    if (verify_content(tokens, currentToken, "for"))
    {
        if (identifier(tokens, currentToken))
        {
            if (tokens[*currentToken].type.compare("NUMBER") == 0)
            {
                eat(tokens, currentToken);
                if (tokens[*currentToken].content.compare("to") == 0)
                {
                    eat(tokens, currentToken);
                    if (isdigit(tokens[*currentToken].content[0]))
                    {
                        eat(tokens, currentToken);
                        if (verify_content(tokens, currentToken, "do"))
                        {
                            return block(tokens, currentToken);
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool whileSentence(vector<Token> tokens, int *currentToken)
{
    if (verify_content(tokens, currentToken, "while"))
    {
        if (verify_content(tokens, currentToken, "("))
        {
            if (!relation(tokens, currentToken))
            {
                error = "Erro na condição do while";
                throw std::invalid_argument(error);
            }
            if (!verify_content(tokens, currentToken, ")"))
            {
                error = "Esperava-se um fecha parêntese após a condição do while";
                throw std::invalid_argument(error);
            }
            if (!verify_content(tokens, currentToken, "do"))
            {
                error = "Esperava-se a palavra 'do' após a condição do while";
                throw std::invalid_argument(error);
            }
            return block(tokens, currentToken);
        }
        error = "Esperava-se um abre parêntese após o while";
        throw std::invalid_argument(error);
    }
    return false;
}

bool sentences(vector<Token> tokens, int *currentToken)
{
    return verify_productions(tokens, currentToken, {callProcedure, varRead, varWrite, ifSentence, forSentence, repeatUntilSentence, whileSentence});
}
