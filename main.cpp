// main.cpp

// Libs
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unordered_set>
#include <unordered_map>
// Functions
#include "getFileInput.cpp"
#include "lexer/lexer.cpp"
#include "./syntactic/syntactic-analyzer.cpp"

using namespace std;

// Função para verificar a compatibilidade do tipo
bool checkTypeCompatibility(const std::vector<std::pair<std::string, std::string>> &tokens)
{
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> scopeTypeMap;
    std::unordered_map<std::string, std::unordered_set<std::string>> declaredVariables;
    std::string currentScope = "global";
    std::string currentType;
    bool declaring = false;
    bool declaringFunction = false;
    std::unordered_set<std::string> functionParameters;

    for (size_t i = 0; i < tokens.size(); ++i)
    {
        std::pair<std::string, std::string> token = tokens[i];
        if (token.first == "function" || token.first == "procedure")
        {
            i++;
            currentScope = tokens[i].first;
            declaredVariables[currentScope] = {};
            declaringFunction = true;
        }
        else if (token.first == "var" && !declaringFunction)
        {
            declaring = true;
        }
        else if (token.first == "(" && declaringFunction)
        {
            declaring = true;
        }
        else if (token.first == ")" && declaringFunction)
        {
            declaringFunction = false;
            declaring = false;
            functionParameters.clear();
        }
        else if ((token.first == "integer" || token.first == "real") && declaring)
        {
            currentType = token.first;
        }
        else if (token.second == "IDENTIFIER" && declaring)
        {
            if ((declaringFunction && functionParameters.count(token.first) > 0) ||
                (!declaringFunction && declaredVariables[currentScope].count(token.first) > 0))
            {
                std::cout << "Erro: Variável '" << token.first << "' declarada mais de uma vez no mesmo escopo.\n";
                return false;
            }
            declaredVariables[currentScope].insert(token.first);
            scopeTypeMap[currentScope][token.first] = currentType;
            if (declaringFunction) functionParameters.insert(token.first);
        }
        else if (token.second == "IDENTIFIER" && !declaring)
        {
            if (declaredVariables[currentScope].count(token.first) == 0)
            {
                std::cout << "Erro: Variável '" << token.first << "' não declarada no escopo atual.\n";
                return false;
            }
        }
        else if (token.second == "OPERATOR" && token.first == ":=")
        {
            if (i > 0 && i + 1 < tokens.size())
            {
                std::string var = tokens[i - 1].first;
                if (scopeTypeMap[currentScope][var] == "integer" && tokens[i + 1].second != "NUMBER")
                {
                    std::cout << "Erro: Incompatibilidade de tipo na atribuição à variável '" << var << "'. Esperado: NUMBER, Obtido: " << tokens[i + 1].second << "\n";
                    return false;
                }
            }
        }
        else if (token.first == "end")
        {
            // Quando saímos de uma função, retornamos ao escopo global
            currentScope = "global";
        }
    }

    return true;
}

// Função para verificar o uso indevido de identificadores reservados
bool checkReservedIdentifierMisuse(const std::vector<std::pair<std::string, std::string>> &tokens)
{
    // Definindo uma lista de identificadores reservados
    std::unordered_set<std::string> reservedIdentifiers = {
        "begin", "end", "var", "integer", "real", "function", "procedure"
        // Adicione mais palavras-chave conforme necessário
    };

    for (size_t i = 0; i < tokens.size(); ++i)
    {
        std::pair<std::string, std::string> token = tokens[i];
        if (token.second == "IDENTIFIER" && reservedIdentifiers.count(token.first) > 0)
        {
            std::cout << "Erro: Uso indevido do identificador reservado '" << token.first << "'.\n";
            return false;
        }
    }

    return true;
}

int main() {
    std::vector<std::pair<std::string, std::string>> tokens = {
        {"var", "KEYWORD"},
        {"real", "IDENTIFIER"},
        {":", "DELIMITER"},
        {"integer", "KEYWORD"},
        {";", "DELIMITER"},
        {"function", "KEYWORD"},
        {"myFunc", "IDENTIFIER"},
        {"(", "DELIMITER"},
        {"var", "KEYWORD"},
        {"id", "IDENTIFIER"},
        {":", "DELIMITER"},
        {"integer", "KEYWORD"},
        {",", "DELIMITER"},
        {"var", "KEYWORD"},
        {"ib", "IDENTIFIER"},
        {":", "DELIMITER"},
        {"integer", "KEYWORD"},
        {")", "DELIMITER"},
        {":", "DELIMITER"},
        {"integer", "KEYWORD"},
        {";", "DELIMITER"},
        {"var", "KEYWORD"},
        {"ia", "IDENTIFIER"},
        {"integer", "KEYWORD"},
        {";", "DELIMITER"},
        {"end", "KEYWORD"},
        {";", "DELIMITER"},
    };

    bool result = checkTypeCompatibility(tokens);

    if (result)
        std::cout << "Nenhum erro encontrado.\n";
    else
        std::cout << "Erro encontrado.\n";
    
    checkReservedIdentifierMisuse(tokens);

    return 0;
}