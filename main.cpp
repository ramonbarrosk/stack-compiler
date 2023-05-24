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

vector<Token> lexical_analysis(string code)
{
    vector<Token> tokens;

    vector<string> keywords = {"program", "begin", "end", "var", "integer", "real", "pilha", "of", "procedure",
                               "function", "read", "write", "for", "to", "do", "repeat", "until", "while", "if", "then", "else"};

    vector<string> operators = {">=", "<=", "<>", "//", "##", ">", "<", "+", "-", "*", "/", ":="};

    vector<string> delimiters = {"(", ")", "[", "]", "{", "}", ";", ",", "#", ":", "."};

    regex pattern("([a-zA-Z_][a-zA-Z_0-9]*|\\b[0-9]*\\.[0-9]+\\b|\\b[0-9]+\\b|[\\(\\)\\,\\=\\;\\#\\+\\-\\*\\/\\>\\<\\:\\.]|>=|<=|<>|//|##|:=)");

    smatch match;

    int lineNumber = 1;

    while (regex_search(code, match, pattern))
    {
        string match_str = match[0];

        // Update line number
        lineNumber += count(match_str.begin(), match_str.end(), '\n');

        // Check for 2-position operators first
        auto op = find(operators.begin(), operators.end(), match_str);
        if (op != operators.end() && match_str.length() == 2)
        {
            tokens.push_back({match_str, "operator", lineNumber});
            code = match.suffix().str();
            continue;
        }

        // Check for remaining types
        string type;
        if (find(keywords.begin(), keywords.end(), match_str) != keywords.end())
            type = "keyword";
        else if (find(delimiters.begin(), delimiters.end(), match_str) != delimiters.end())
            type = "delimiter";
        else if (regex_match(match_str, regex("[a-zA-Z_][a-zA-Z_0-9]*")))
            type = "identifier";
        else if (regex_match(match_str, regex("\\b[0-9]*\\.[0-9]+\\b")))
            type = "real";
        else if (regex_match(match_str, regex("\\b[0-9]+\\b")))
            type = "integer";
        else if (op != operators.end())
            type = "operator";

        tokens.push_back({match_str, type, lineNumber});

        // Remove the identified token from the code
        code = match.suffix().str();
    }

    return tokens;
}

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
void checkReservedIdentifierMisuse(vector<Token> &tokens)
{
    vector<string> reservedIdentifiers = {"program", "begin", "end", "var", "integer", "real", "pilha", "of", "procedure", "concatena",
                                          "function", "read", "write", "for", "to", "do", "repeat", "until", "while", "if", "then", "else"};

    for (size_t i = 0; i < tokens.size() - 1; i++)
    {
        if (tokens[i].content == "var" && std::find(reservedIdentifiers.begin(), reservedIdentifiers.end(), tokens[i + 1].content) != reservedIdentifiers.end())
        {
            cout << "Erro: Uso indevido do identificador reservado '" << tokens[i + 1].content << "' na linha " << tokens[i].line << endl;
        }
    }
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
    std::unordered_map<std::string, IdentifierType> symbolTable;

    std::vector<std::string> reservedIdentifiers = {"program", "begin", "end", "var", "integer", "real", "pilha", "of", "procedure", "concatena",
                                                    "function", "read", "write", "for", "to", "do", "repeat", "until", "while", "if", "then", "else"};

    bool hasErrors = false;

    for (size_t i = 0; i < tokens.size(); ++i)
    {
        const std::string &content = tokens[i].first;
        const std::string &type = tokens[i].second;

        if (type == "identifier" && i > 0 && tokens[i - 1].first == "var")
        {
            symbolTable[content] = VARIABLE;
        }
        else if (type == "identifier" && i > 0 && tokens[i - 1].first == "function")
        {
            symbolTable[content] = FUNCTION;
        }

        if (type == "identifier" && i + 1 < tokens.size() && tokens[i + 1].first == "(")
        {
            if (symbolTable.count(content) == 0 || symbolTable[content] != FUNCTION)
            {
                if (std::find(reservedIdentifiers.begin(), reservedIdentifiers.end(), content) == reservedIdentifiers.end())
                {
                    std::cout << "Error: Undeclared function call '" << content << "'.\n";
                    hasErrors = true;
                }
            }
        }
    }

    return !hasErrors;
}

// Adicione esta estrutura à sua lista de estruturas de dados
struct FunctionSignature
{
    std::string name;
    std::vector<std::string> parameters;
};

// Função para verificar correspondência de parâmetros
void checkParameterMismatch(const std::vector<std::pair<std::string, std::string>> &tokens)
{
    std::unordered_map<std::string, int> functionParameters;

    for (size_t i = 0; i < tokens.size(); ++i)
    {
        const std::string &content = tokens[i].first;
        const std::string &type = tokens[i].second;

        if (type == "identifier" && i > 0 && tokens[i - 1].first == "function")
        {
            std::string functionName = content;
            int paramCount = 0;
            int paramVarCount = 0;

            // Encontra o ponto inicial dos parâmetros
            size_t j = i + 1;
            while (j < tokens.size() && tokens[j].first != "(")
            {
                ++j;
            }

            if (j < tokens.size())
            {
                // Conta as variáveis e verifica se não são palavras-chave
                ++j;
                while (j < tokens.size() && tokens[j].first != ")")
                {
                    if (tokens[j].first == "var")
                    {
                        ++paramVarCount;
                        if (j + 1 < tokens.size() && tokens[j + 1].second == "identifier" && !isAKeyword(tokens[j + 1].first))
                        {
                            ++paramCount;
                        }
                    }
                    ++j;
                }
            }

            // Armazena o nome da função e a contagem de parâmetros
            functionParameters[functionName] = paramCount;
        }
        else if (type == "identifier" && !isAKeyword(content) && i + 1 < tokens.size() && tokens[i + 1].first == "(")
        {
            std::string functionName = content;
            int paramCount = 0;

            // Encontra o nome da função nos parâmetros armazenados
            if (functionParameters.count(functionName) > 0)
            {
                paramCount = functionParameters[functionName];
            }

            // Compara a contagem de parâmetros armazenada com a contagem de parâmetros atual
            size_t j = i + 2;         // Pula os parênteses da chamada da função
            int actualParamCount = 1; // Começa com 1 para o próprio nome da função
            if (tokens[j].first == ")")
            {
                std::cout << "Erro: a função não possui os parâmetros necessários, a função: " << functionName << " precisa de " << paramCount << " parâmetros.\n";
                return;
            }
            while (j < tokens.size() && tokens[j].first != ")")
            {
                cout << tokens[j].first << endl;
                if (tokens[j].first == ",")
                {
                    ++actualParamCount;
                }
                ++j;
            }

            if (paramCount != actualParamCount)
            {
                std::cout << "Erro: Mismatch de parâmetros na chamada da função '" << functionName << "'.\n";
            }
        }
    }
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
    string fileName = "example-file.txt";
    vector<string> fileInput = getFileInput(fileName);

    std::string combinedInput;
    for (const auto &line : fileInput)
    {
        combinedInput += line + "\n"; // Preserving line breaks
    }

    vector<Token> tokens = lexical_analysis(combinedInput);
    // for (const auto &token : tokens)
    // {
    //     cout << "Content: " << token.content << ", Type: " << token.type << endl;
    // }

    vector<pair<string, string>> pairs;
    for (const auto &token : tokens)
    {
        pairs.push_back(make_pair(token.content, token.type));
    }

    checkTypeCompatibility(pairs);
    checkReservedIdentifierMisuse(tokens);
    checkFunctionExistence(pairs);
    checkParameterMismatch(pairs);
    computeUndeclaredVariables(pairs);

    return 0;
}