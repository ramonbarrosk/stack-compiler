#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <regex>

#include "../syntactic-analyzer.h"
#include "../utils/eat.cpp"
#include "../utils/verify-productions.cpp"

bool body(vector<Token> tokens, int* currentToken) {
    return verify_productions(tokens, currentToken, {declaration, isBeginAndEnd});
}
// o return do body vai ficar mais ou menos assim:
// return verify_productions(tokens, currentToken, {declaration, routines, begin&end}); 
