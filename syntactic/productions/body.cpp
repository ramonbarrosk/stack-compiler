#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <regex>

#include "../syntactic-analyzer.h"
#include "../utils/eat.cpp"
#include "../utils/verify-productions.cpp"

bool body(vector<Token> tokens, int* currentToken) {
    // tirei o declaration por enquanto do body, essa merda funciona pra o block, mas pra ca n. é o pijas.
    return verify_productions(tokens, currentToken, {isBeginAndEnd});
}
// o return do body vai ficar mais ou menos assim:
// return verify_productions(tokens, currentToken, {declaration, routines, begin&end}); 
