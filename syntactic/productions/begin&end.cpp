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
    if (verify_content(tokens, currentToken, "begin")) {
        return block(tokens, currentToken);
    }

    return false;
}

bool isBeginAndEnd(vector<Token> tokens, int* currentToken) {
    cout << "\n" << *currentToken << " ola\n";
    return verify_productions(tokens, currentToken, {checkBeginEnd});
}