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
        else if (token.second == "IDENTIFIER" && declaring && i + 1 < tokens.size() && tokens[i + 1].first == ":") // variable declaration
        {
            if ((declaringFunction && functionParameters.count(token.first) > 0) ||
                (!declaringFunction && declaredVariables[currentScope].count(token.first) > 0))
            {
                std::cout << "Erro: Variável '" << token.first << "' declarada mais de uma vez no mesmo escopo.\n";
                return false;
            }
            declaredVariables[currentScope].insert(token.first);
            scopeTypeMap[currentScope][token.first] = currentType;
            if (declaringFunction)
                functionParameters.insert(token.first);
        }
        else if (token.second == "IDENTIFIER" && !(declaring && i + 1 < tokens.size() && tokens[i + 1].first == ":")) // variable access
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
                std::string assignedType = scopeTypeMap[currentScope][var];
                std::string valueType = tokens[i + 1].second;

                if (assignedType == "integer" && valueType != "NUMBER")
                {
                    std::cout << "Erro: Incompatibilidade de tipo na atribuição à variável '" << var << "'. Esperado: NUMBER, Obtido: " << valueType << "\n";
                    return false;
                }
                else if (assignedType == "real" && valueType != "NUMBER")
                {
                    std::cout << "Erro: Incompatibilidade de tipo na atribuição à variável '" << var << "'. Esperado: NUMBER, Obtido: " << valueType << "\n";
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

// Adicione à sua lista de tipos de identificador
enum IdentifierType
{
    VARIABLE,
    FUNCTION
};

// Função para verificar a existência de funções
bool checkFunctionExistence(const std::vector<std::pair<std::string, std::string>> &tokens)
{
    // Criação da tabela de símbolos
    std::unordered_map<std::string, IdentifierType> symbolTable;

    for (size_t i = 0; i < tokens.size(); ++i)
    {
        std::pair<std::string, std::string> token = tokens[i];

        // Se encontrarmos uma declaração de variável ou função, adicionamos ao mapa
        if (token.second == "IDENTIFIER" && i > 0 && tokens[i - 1].first == "var")
        {
            symbolTable[token.first] = VARIABLE;
        }
        else if (token.second == "IDENTIFIER" && i > 0 && tokens[i - 1].first == "function")
        {
            symbolTable[token.first] = FUNCTION;
        }

        // Se encontrarmos uma chamada de função, verificamos se a função existe na tabela de símbolos
        if (token.second == "IDENTIFIER" && i + 1 < tokens.size() && tokens[i + 1].first == "(")
        {
            if (symbolTable.count(token.first) == 0 || symbolTable[token.first] != FUNCTION)
            {
                std::cout << "Erro: Chamada de função não declarada '" << token.first << "'.\n";
                return false;
            }
        }
    }

    return true;
}

// Adicione esta estrutura à sua lista de estruturas de dados
struct FunctionSignature
{
    std::string name;
    std::vector<std::string> parameters;
};

// Função para verificar correspondência de parâmetros
bool checkParameterMismatch(const std::vector<std::pair<std::string, std::string>> &tokens) {
    unordered_map<string, vector<string>> functionParameters;
    unordered_set<string> declaredFunctions;
    vector<string> currentParameters;
    string currentFunction;
    bool declaringFunction = false;
    bool callingFunction = false;

    for (const auto &token : tokens) {
        if (token.first == "function") {
            declaringFunction = true;
        }
        else if (declaringFunction && token.second == "IDENTIFIER") {
            currentFunction = token.first;
            declaredFunctions.insert(currentFunction);
        }
        else if (declaringFunction && token.second == "DELIMITER" && token.first == "(") {
            currentParameters.clear();
        }
        else if (declaringFunction && token.second == "IDENTIFIER") {
            currentParameters.push_back(token.first);
        }
        else if (declaringFunction && token.second == "DELIMITER" && token.first == ")") {
            functionParameters[currentFunction] = currentParameters;
            declaringFunction = false;
        }
        else if (!declaringFunction && declaredFunctions.count(token.first) > 0) {
            callingFunction = true;
            currentFunction = token.first;
        }
        else if (callingFunction && token.second == "NUMBER") {
            if (functionParameters[currentFunction].size() < 1) {
                cout << "Erro: Mismatch de parâmetros na chamada da função '" << currentFunction << "'.\n";
                return false;
            }
            functionParameters[currentFunction].erase(functionParameters[currentFunction].begin());
        }
        else if (callingFunction && token.second == "DELIMITER" && token.first == ")") {
            if (!functionParameters[currentFunction].empty()) {
                cout << "Erro: Mismatch de parâmetros na chamada da função '" << currentFunction << "'.\n";
                return false;
            }
            callingFunction = false;
        }
    }

    return true;
}

bool computeUndeclaredVariables(const std::vector<std::pair<std::string, std::string>> &tokens)
{
    std::unordered_set<std::string> declaredVariables;
    std::unordered_set<std::string> undeclaredVariables;

    bool declaring = false;
    bool declaringFunction = false;

    for (const auto &token : tokens)
    {
        if (token.first == "var")
        {
            declaring = true;
        }
        else if (token.first == ";")
        {
            declaring = false;
        }
        else if (token.first == "function" || token.first == "procedure")
        {
            declaringFunction = true;
        }
        else if (token.first == "(")
        {
            declaring = true;
        }
        else if (token.first == ")")
        {
            declaringFunction = false;
            declaring = false;
        }
        else if (declaring && token.second == "IDENTIFIER")
        {
            declaredVariables.insert(token.first);
        }
        else if (!declaringFunction && token.second == "IDENTIFIER" && !declaring && declaredVariables.count(token.first) == 0)
        {
            undeclaredVariables.insert(token.first);
        }
    }

    // Printa se tiver
    if (!undeclaredVariables.empty())
    {
        std::cout << "Identificadores não declarados:\n";
        for (const auto &var : undeclaredVariables)
        {
            std::cout << var << std::endl;
        }
        return true;
    }
    else
    {
        std::cout << "Identificadores estão okay.\n";
    }

    return false;
}


int main()
{
    std::vector<std::pair<std::string, std::string>> tokens = {
        {"var", "KEYWORD"},
        {"a", "IDENTIFIER"},
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
        {"begin", "KEYWORD"},
        {"var", "KEYWORD"},
        {"ia", "IDENTIFIER"},
        {":", "DELIMITER"},
        {"integer", "KEYWORD"},
        {";", "DELIMITER"},
        {"end", "KEYWORD"},
        {";", "DELIMITER"},
        {"myFunc", "IDENTIFIER"},
        {"(", "DELIMITER"},
        {"3", "NUMBER"},
        {",", "DELIMITER"},
        {"2", "NUMBER"},
        {")", "DELIMITER"},
    };

    checkTypeCompatibility(tokens);
    checkReservedIdentifierMisuse(tokens);
    checkFunctionExistence(tokens);
    checkParameterMismatch(tokens);
    computeUndeclaredVariables(tokens);

    return 0;
}