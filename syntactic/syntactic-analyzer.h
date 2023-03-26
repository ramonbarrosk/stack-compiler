#include <string>
#include <cstring>
#include <vector>

#pragma once

#include "../bnf/types.h"

using namespace std;

void syntacticAnalyzer(vector<Token> tokens);

bool startProgram(vector<Token> tokens, int* currentToken);

// verifica se o "header" é válido. (<program> id;)
bool programIdentifier(vector<Token> tokens, int* currentToken);

// identificadores
bool identifier(vector<Token> tokens, int* currentToken);

// verifica se o corpo é válido
bool body(vector<Token> tokens, int* currentToken);

// variaveis
bool declaration(vector<Token> tokens, int* currentToken);
bool checkVarDeclaration(vector<Token> tokens, int* currentToken);
bool hasIdAfterVar(vector<Token> tokens, int* currentToken);