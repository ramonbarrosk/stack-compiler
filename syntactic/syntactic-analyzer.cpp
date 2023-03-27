#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <regex>
#include <stack>

#include "syntactic-analyzer.h"

#include "./productions/_index.cpp"

using namespace std;

void syntacticAnalyzer(vector<Token> tokens) {
    int currentToken = 0;

    Token token;
    token.content = ".";
    token.type = "END_PROGRAM";

    tokens.push_back(token);
    bool validProgram = startProgram(tokens, &currentToken);
}

bool startProgram(vector<Token> tokens, int* currentToken) {
    bool startProgram = programIdentifier(tokens, currentToken);
    if (startProgram) {
        body(tokens, currentToken);
    }
    cout << "\nvalue: " << tokens[*currentToken].content << "\n";
    cout << "CurrentToken: " << *currentToken << "\n";

    if (tokens[*currentToken].type.compare("END_PROGRAM") == 0) {
        return true;
    }
    return false;
}
