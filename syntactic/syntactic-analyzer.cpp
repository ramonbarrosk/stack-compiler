#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <regex>
#include <stack>

#include "syntactic-analyzer.h"

#include "./productions/_index.cpp"
#include "./symbol-table.cpp"

void syntacticAnalyzer(vector<Token> tokens)
{
    int currentToken = 0;

    Token token;
    token.content = ".";
    token.type = "END_PROGRAM";

    tokens.push_back(token);
    bool validProgram = startProgram(tokens, &currentToken);
}

bool startProgram(vector<Token> tokens, int *currentToken)
{
    bool startProgram = programIdentifier(tokens, currentToken);
    bool fez_o_sintatico_meu_deus;
    if (startProgram)
    {
        fez_o_sintatico_meu_deus = body(tokens, currentToken);
    }
    // cout << "\nvalue: " << tokens[*currentToken].content << "\n";
    // cout << "CurrentToken: " << *currentToken << "\n";
    if (fez_o_sintatico_meu_deus)
    {
        vector<var_scope> tabela = tabela_de_simbolos(tokens);
        cout << "TABELA DE SÍMBOLOS: \n";
        for (auto entry : tabela)
        {
            cout << entry.name << " | " << entry.scope.name << " [" << entry.scope.type << "]\n";
        }
    }
    if (tokens[*currentToken].type.compare("END_PROGRAM") == 0)
    {
        return true;
    }
    return false;
}
