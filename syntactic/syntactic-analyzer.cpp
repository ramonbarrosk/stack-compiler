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

    cout << "CurrentToken: " << currentToken << "\n";
}

bool startProgram(vector<Token> tokens, int* currentToken) {
    bool startProgram = programIdentifier(tokens, currentToken);
    if (startProgram) {
        body(tokens, currentToken);
    }
    if (tokens[*currentToken].type.compare("END_PROGRAM") == 0) {
        return true;
    }
    return false;
}
