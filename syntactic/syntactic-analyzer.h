#include <string>
#include <cstring>
#include <vector>

#pragma once

#include "../bnf/types.h"

using namespace std;

string error;
string funcError;

regex types ("integer|pilha_of_real|pilha_of_integer|real");


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
bool relation(vector<Token> tokens, int *currentToken);
// função/rotina
bool parameters(vector<Token> tokens, int *currentToken);
bool lista_id(vector<Token> tokens, int *currentToken);
bool cont_lista_id(vector<Token> tokens, int *currentToken);
bool cont_lista_parametros(vector<Token> tokens, int *currentToken);
bool functionVerifier(vector<Token> tokens, int *currentToken);

bool isAFunction(vector<Token> tokens, int *currentToken);
bool isAProcedure(vector<Token> tokens, int *currentToken);

bool bnfProcedure(vector<Token> tokens, int *currentToken);
bool bnffunction(vector<Token> tokens, int *currentToken);

bool callProcedure(vector<Token> tokens, int *currentToken);
bool routines(vector<Token> tokens, int *currentToken);

// argumentos 
bool args(vector<Token> tokens, int *currentToken);
bool argumentos(vector<Token> tokens, int *currentToken);
bool lista_arg(vector<Token> tokens, int *currentToken);
bool cont_lista_arg(vector<Token> tokens, int *currentToken);

bool expr(vector<Token> tokens, int *currentToken);
