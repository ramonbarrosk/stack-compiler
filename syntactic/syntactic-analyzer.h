#include <string>
#include <cstring>
#include <vector>

#pragma once

#include "../bnf/types.h"

using namespace std;

void syntacticAnalyzer(vector<Token> tokens);

bool startProgram(vector<Token> tokens, int* currentToken);
bool programIdentifier(vector<Token> tokens, int* currentToken);
bool identifier(vector<Token> tokens, int* currentToken);