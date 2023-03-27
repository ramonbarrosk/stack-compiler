#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <regex>
#include <stdexcept>

#include "../syntactic-analyzer.h"
#include "../utils/eat.cpp"
#include "../utils/verify-productions.cpp"

bool checkBeginEnd(vector<Token> tokens, int* currentToken) {
    // Verifica se o código começa com "begin"
    cout << " executei ";
    if (verify_content(tokens, currentToken, "begin")) {
       if (eat(tokens, currentToken)) return true;
    }

    return false;
}

bool isBeginAndEnd(vector<Token> tokens, int* currentToken) {
    return verify_productions(tokens, currentToken, {checkBeginEnd});
}