#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <regex>

#pragma once

using namespace std;

vector<string> splitCommandsByDelimiter(vector<string> script, char delim)
{
    // separa o código em linhas
    string linha;
    vector<string> tokens;
    for (int i = 0; i < script.size(); i++)
    {
        istringstream iss(script[i]);
        // loop para processar cada linha
        while (getline(iss, linha))
        {
            // separa a linha em palavras
            istringstream iss_linha(linha);
            string palavra;
            while (iss_linha >> palavra)
            {
                // adiciona a palavra à lista de tokens
                string token;
                for (char c : palavra)
                {
                    if (c == ';' || c == '(' || c == ')' || c == '[' || c == ']' || c == '#' || c == '{' || c == '}' || c == ',')
                    {
                        // separa o caractere especial em um token separado
                        if (!token.empty())
                        {
                            tokens.push_back(token);
                            token.clear();
                        }
                        tokens.push_back(string(1, c));
                    }
                    else if (c == '.' && i + 1 < palavra.size() && isdigit(palavra[i + 1]))
                    {
                        // verifica se é um número real
                        token += c;
                    }
                    else
                    {
                        token += c;
                    }
                }
                if (!token.empty())
                {
                    tokens.push_back(token);
                }
            }
        }
    }
    return tokens;
}