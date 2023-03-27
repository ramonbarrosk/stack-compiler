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

// blocos de código
bool isBeginAndEnd(vector<Token> tokens, int* currentToken);
bool block(vector<Token> tokens, int* currentToken);

bool operation(vector<Token> tokens, int* currentToken);

// sentenças
bool varRead(vector<Token> tokens, int *currentToken);
bool varWrite(vector<Token> tokens, int *currentToken);
bool sentences(vector<Token> tokens, int *currentToken);
