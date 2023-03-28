#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <regex>

#include "../syntactic-analyzer.h"
#include "../utils/eat.cpp"
#include "../utils/verify-loop-productions.cpp"
#include "../utils/verify-productions.cpp"

bool routines(vector<Token> tokens, int *currentToken) {
    return verify_productions(tokens, currentToken, {bnffunction, bnfProcedure});
}