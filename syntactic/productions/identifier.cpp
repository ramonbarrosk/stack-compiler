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

bool verifyIfIsAId(vector<Token> tokens, int* currentToken)
{
    if (tokens[*currentToken].type.compare(IDENTIFIER) == 0) {
        if (eat(tokens, currentToken)) return true;
    }
    return false;
}

bool identifier(vector<Token> tokens, int* currentToken)
{
    return verify_productions(tokens, currentToken, {verifyIfIsAId});
}
