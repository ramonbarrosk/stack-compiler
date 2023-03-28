#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <regex>
#include <stack>

#include "./productions/_index.cpp"

using namespace std;

typedef struct scope
{
    string name;
    string type;
} Scope;

typedef struct var_scope
{
    string name;
    Scope scope;
} var_scope;

Scope newScope(string name, string type)
{
    Scope novo;
    novo.name = name;
    novo.type = type;

    return novo;
}

var_scope newVarScope(string name, Scope scope)
{
    var_scope nova;
    nova.name = name;
    nova.scope = scope;

    return nova;
}

// verify content

void scope_type(vector<Token> tokens, vector<Scope> keywords, stack<Scope> *escopos, int *i)
{
    for (auto key : keywords)
    {
        if (tokens[*i].content.compare(key.name) == 0)
        {
            if (key.name.compare("function") == 0)
            {
                escopos->push({tokens[*i + 1].content, key.type});
                *i = *i + 2;
                return;
            }
            escopos->push({key});
            return;
        }
    }
}

vector<var_scope> tabela_de_simbolos(vector<Token> tokens)
{
    vector<var_scope> symbols_table;

    vector<Scope> block_keys = {
        {"function", "function"},
        {"if", "if"},
        {"else", "else"},
        {"for", "for"},
        {"while", "while"},
        {"begin", "begin"},
        {"repeat", "repeat"},
    };

    stack<Scope> escopos;

    Scope padrao = newScope("main", "main"); //main é o escopo mais externo

    for (int i = 0; i < tokens.size(); i++)
    {
        scope_type(tokens, block_keys, &escopos, &i);

        if (tokens[i].type.compare(IDENTIFIER) == 0)
        {
            if (escopos.size() == 0)
            {
                symbols_table.push_back({tokens[i].content, {"main", "main"}});
            }
            else
            {
                symbols_table.push_back({tokens[i].content, {escopos.top().name, escopos.top().type}});
            }
        }

        if (tokens[i].content.compare("end") == 0)
        {
            escopos.pop();
        }
    }

    return symbols_table;
}