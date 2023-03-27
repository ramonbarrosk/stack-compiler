#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <regex>

#include "../syntactic-analyzer.h"
#include "../utils/eat.cpp"
#include "../utils/verify-productions.cpp"

#pragma once

using namespace std;

bool isProgramKeyword(vector<Token> tokens, int *currentToken)
{
    if (tokens[*currentToken].content.compare("program") == 0)
    {
        if (eat(tokens, currentToken))
            return true;
    }
    return false;
}

bool hasIdAfterProgramKeyword(vector<Token> tokens, int *currentToken)
{
    if (tokens[*currentToken].type.compare(IDENTIFIER) == 0)
    {
        if (eat(tokens, currentToken))
            return true;
    }
    return false;
}

bool hasDelimiterAfterProgramId(vector<Token> tokens, int *currentToken) {
    return verify_content(tokens, currentToken, ";");
}

bool checkValidHeader(vector<Token> tokens, int *currentToken) {
    if (isProgramKeyword(tokens, currentToken)) {
        if (hasIdAfterProgramKeyword(tokens, currentToken)) {
            return hasDelimiterAfterProgramId(tokens, currentToken);
        }
    }
    return false;
}

bool programIdentifier(vector<Token> tokens, int *currentToken)
{
    return verify_productions(tokens, currentToken, {checkValidHeader});
}