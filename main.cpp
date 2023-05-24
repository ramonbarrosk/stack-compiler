// main.cpp

// Bibliotecas
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <unordered_set>
#include <unordered_map>
// Funções
#include "getFileInput.cpp"
#include "lexer/lexer.cpp"
#include "./syntactic/syntactic-analyzer.cpp"

using namespace std;

#include <sstream>

// Função para análise léxica
std::vector<Token> lexical_analysis(const std::string& code)
{
    std::vector<Token> tokens;

    std::vector<std::string> keywords = {"program", "begin", "end", "var", "integer", "real", "pilha", "of", "procedure",
                                         "function", "read", "write", "for", "to", "do", "repeat", "until", "while", "if", "then", "else"};

    std::vector<std::string> operators = {">=", "<=", "<>", "//", "##", ">", "<", "+", "-", "*", "/", ":="};

    std::vector<std::string> delimiters = {"(", ")", "[", "]", "{", "}", ";", ",", "#", ":", "."};

    std::regex pattern("([a-zA-Z_][a-zA-Z_0-9]*|\\b[0-9]*\\.[0-9]+\\b|\\b[0-9]+\\b|[\\(\\)\\,\\=\\;\\#\\+\\-\\*\\/\\>\\<\\:\\.]|>=|<=|<>|//|##|:=)");

    std::smatch match;

    std::stringstream ss(code);
    std::string line;
    int lineNumber = 0;

    while (std::getline(ss, line))
    {
        lineNumber++;
        while (std::regex_search(line, match, pattern))
        {
            std::string match_str = match[0];

            // Check for 2-position operators first
            auto op = std::find(operators.begin(), operators.end(), match_str);
            if (op != operators.end() && match_str.length() == 2)
            {
                tokens.push_back({match_str, "operator", "", "", lineNumber});
                line = match.suffix().str();
                continue;
            }

            // Check for remaining types
            std::string type;
            if (std::find(keywords.begin(), keywords.end(), match_str) != keywords.end())
                type = "keyword";
            else if (std::find(delimiters.begin(), delimiters.end(), match_str) != delimiters.end())
                type = "delimiter";
            else if (std::regex_match(match_str, std::regex("[a-zA-Z_][a-zA-Z_0-9]*")))
                type = "identifier";
            else if (std::regex_match(match_str, std::regex("\\b[0-9]*\\.[0-9]+\\b")))
            {
                type = "real";
                float value = std::stof(match_str);
                tokens.push_back({match_str, type, "", std::to_string(value), lineNumber});
            }
            else if (std::regex_match(match_str, std::regex("\\b[0-9]+\\b")))
            {
                type = "integer";
                int value = std::stoi(match_str);
                tokens.push_back({match_str, type, "", std::to_string(value), lineNumber});
            }
            else if (op != operators.end())
                type = "operator";

            if (type != "real" && type != "integer")
                tokens.push_back({match_str, type, "", "", lineNumber});

            // Remove the identified token from the line
            line = match.suffix().str();
        }
    }

    return tokens;
}


// Função para verificar a compatibilidade de tipos
bool checkTypeCompatibility(const std::vector<Token>& tokens)
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
        const Token& token = tokens[i];
        const std::string& tokenContent = token.content;
        const std::string& tokenType = token.type;
        const int line = token.line;

        if (tokenContent == "function" || tokenContent == "procedure")
        {
            i++;
            currentScope = tokens[i].content;
            declaredVariables[currentScope] = {};
            declaringFunction = true;
        }
        else if (tokenContent == "var" && !declaringFunction)
        {
            declaring = true;
        }
        else if (tokenContent == "(" && declaringFunction)
        {
            declaring = true;
        }
        else if (tokenContent == ")" && declaringFunction)
        {
            declaringFunction = false;
            declaring = false;
            functionParameters.clear();
        }
        else if ((tokenContent == "integer" || tokenContent == "real") && declaring)
        {
            currentType = tokenContent;
        }
        else if (tokenType == "IDENTIFIER" && declaring && i + 1 < tokens.size() && tokens[i + 1].content == ":") // declaração de variável
        {
            if ((declaringFunction && functionParameters.count(tokenContent) > 0) ||
                (!declaringFunction && declaredVariables[currentScope].count(tokenContent) > 0))
            {
                std::cout << "Erro: Variável '" << tokenContent << "' declarada mais de uma vez no mesmo escopo na linha " << line << ".\n";
                return false;
            }
            declaredVariables[currentScope].insert(tokenContent);
            scopeTypeMap[currentScope][tokenContent] = currentType;
            if (declaringFunction)
                functionParameters.insert(tokenContent);
        }
        else if (tokenType == "IDENTIFIER" && !(declaring && i + 1 < tokens.size() && tokens[i + 1].content == ":")) // acesso à variável
        {
            if (declaredVariables[currentScope].count(tokenContent) == 0)
            {
                std::cout << "Erro: Variável '" << tokenContent << "' não declarada no escopo atual na linha " << line << ".\n";
                return false;
            }
        }
        else if (tokenType == "OPERATOR" && tokenContent == ":=")
        {
            if (i > 0 && i + 1 < tokens.size())
            {
                std::string var = tokens[i - 1].content;
                std::string assignedType = scopeTypeMap[currentScope][var];
                std::string valueType = tokens[i + 1].type;

                if (assignedType == "integer" && valueType != "NUMBER")
                {
                    std::cout << "Erro: Incompatibilidade de tipo na atribuição à variável '" << var << "'. Esperado: NUMBER, Obtido: " << valueType << " na linha " << line << ".\n";
                    return false;
                }
                else if (assignedType == "real" && valueType != "NUMBER")
                {
                    std::cout << "Erro: Incompatibilidade de tipo na atribuição à variável '" << var << "'. Esperado: NUMBER, Obtido: " << valueType << " na linha " << line << ".\n";
                    return false;
                }
            }
        }
        else if (tokenContent == "end")
        {
            // Ao sair de uma função, voltamos ao escopo global
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
            cout << "Erro: Uso indevido do identificador reservado '" << tokens[i + 1].content << "' na linha " << tokens[i].line << ".\n";
        }
    }
}

// Adicione à sua lista de tipos de identificador
enum IdentifierType { INTEGER, REAL, FUNCTION, VARIABLE, UNDEFINED };

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
                    std::cout << "Erro: Chamada de função não declarada '" << content << "'.\n";
                    hasErrors = true;
                }
            }
        }
    }

    return !hasErrors;
}


bool checkFunctionAndValueType(const std::vector<Token>& tokens)
{
    std::unordered_map<std::string, IdentifierType> symbolTable;

    std::unordered_map<IdentifierType, std::string> checkEnum = {{INTEGER, "integer"}, {REAL, "real"}};

    std::vector<std::string> reservedIdentifiers = {"program", "begin", "end", "var", "integer", "real", "pilha", "of", "procedure", "concatena",
                                                    "function", "read", "write", "for", "to", "do", "repeat", "until", "while", "if", "then", "else"};

    bool hasErrors = false;

    for (size_t i = 0; i < tokens.size(); ++i)
    {
        const Token& token = tokens[i];
        const std::string &content = token.content;
        const std::string &type = token.type;
        const int line = token.line;

        if (type == "identifier" && i > 0 && tokens[i - 1].content == "var")
        {
            if (i + 2 < tokens.size() && tokens[i + 2].content == "integer")
            {   
                symbolTable[content] = INTEGER;
            }
            else if (i + 2 < tokens.size() && tokens[i + 2].content == "real")
            {
                symbolTable[content] = REAL;
            }
            else
            {
                symbolTable[content] = VARIABLE;
            }
        }
        else if ((type == "integer" || type == "real") && i >= 2 && tokens[i - 2].type == "identifier" && tokens[i - 3].content != "(")
        {
            const std::string& identifierContent = tokens[i - 2].content;
            IdentifierType idType = symbolTable[identifierContent];

            if (idType == INTEGER && type == "real")
            {
                std::cout << "Erro: Tipo de valor incorreto para '" << identifierContent << "' na linha " << line << ".\n";
                hasErrors = true;
            }
        }
        else if (type == "operator" && (content == "+" || content == "-" || content == "*" || content == "/"))
        {
            if (i + 4 < tokens.size())
            {
                const std::string& firstParamContent = tokens[i + 2].content;
                IdentifierType firstParamIdType = symbolTable[firstParamContent];
                const std::string& type1 = tokens[i + 2].type;

                const std::string& secondParamContent = tokens[i + 4].content;
                IdentifierType secondParamIdType = symbolTable[secondParamContent];
                const std::string& type2 = tokens[i + 4].type;

                if (type1 == "identifier")
                {
                    if (type2 == "identifier")
                    {
                        if (firstParamIdType != secondParamIdType)
                        {
                            std::cout << "Você está tentando operar com tipos inválidos na linha " << line << "...\n";
                            hasErrors = true;
                        }
                    }
                    else
                    {
                        std::string currentType = checkEnum[firstParamIdType];
                        if (currentType != type2)
                        {
                            std::cout << "Você está tentando operar com tipos inválidos na linha " << line << "...\n";
                            hasErrors = true;
                        }
                    }
                }
                else
                {
                    if (type2 == "identifier")
                    {
                        std::string currentType = checkEnum[secondParamIdType];
                        if (type1 != currentType)
                        {
                            std::cout << "Você está tentando operar com tipos inválidos na linha " << line << "...\n";
                            hasErrors = true;
                        }
                    }
                    else
                    {
                        if (type1 != type2)
                        {
                            std::cout << "Você está tentando operar com tipos inválidos na linha " << line << "...\n";
                            hasErrors = true;
                        }
                    }
                }
            }
        }
    }

    return !hasErrors;
}


struct FunctionSignature
{
    std::string name;
    std::vector<std::string> parameters;
};

// Função para verificar correspondência de parâmetros
void checkParameterMismatch(const std::vector<Token>& tokens)
{
    std::unordered_map<std::string, int> functionParameters;

    for (size_t i = 0; i < tokens.size(); ++i)
    {
        const Token& token = tokens[i];
        const std::string& content = token.content;
        const std::string& type = token.type;
        const int line = token.line;

        if (type == "identifier" && i > 0 && tokens[i - 1].content == "function")
        {
            std::string functionName = content;
            int paramCount = 0;
            int paramVarCount = 0;

            // Encontra o ponto inicial dos parâmetros
            size_t j = i + 1;
            while (j < tokens.size() && tokens[j].content != "(")
            {
                ++j;
            }

            if (j < tokens.size())
            {
                // Conta as variáveis e verifica se não são palavras-chave
                ++j;
                while (j < tokens.size() && tokens[j].content != ")")
                {
                    if (tokens[j].content == "var")
                    {
                        ++paramVarCount;
                        if (j + 1 < tokens.size() && tokens[j + 1].type == "identifier" && !isAKeyword(tokens[j + 1].content))
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
        else if (type == "identifier" && !isAKeyword(content) && i + 1 < tokens.size() && tokens[i + 1].content == "(")
        {
            std::string functionName = content;
            int paramCount = 0;

            // Encontra o nome da função nos parâmetros armazenados
            if (functionParameters.count(functionName) > 0)
            {
                paramCount = functionParameters[functionName];
            }

            // Verifica se a função foi declarada
            bool functionDeclared = false;
            for (size_t j = 0; j < tokens.size(); ++j)
            {
                if (tokens[j].content == "function" && j + 1 < tokens.size() && tokens[j + 1].content == functionName)
                {
                    functionDeclared = true;
                    break;
                }
            }

            // Compara a contagem de parâmetros armazenada com a contagem de parâmetros atual
            size_t j = i + 2;         // Pula os parênteses da chamada da função
            int actualParamCount = 1; // Começa com 1 para o próprio nome da função
            if (tokens[j].content == ")")
            {
                if (!functionDeclared)
                {
                    std::cout << "A função '" << functionName << "' não foi declarada. Linha: " << line << std::endl;
                    return;
                }
                else if (paramCount != 0)
                {
                    std::cout << "Erro: A função '" << functionName << "' não possui os parâmetros necessários. Linha: " << line << std::endl;
                    return;
                }
            }
            while (j < tokens.size() && tokens[j].content != ")")
            {
                if (tokens[j].content == ",")
                {
                    ++actualParamCount;
                }
                ++j;
            }

            if (paramCount != actualParamCount)
            {
                std::cout << "Erro: Mismatch de parâmetros na chamada da função '" << functionName << "'. Linha: " << line << std::endl;
            }
        }
    }
}

void computeUndeclaredVariables(const std::vector<Token>& tokens) 
{
    std::unordered_map<std::string, IdentifierType> symbolTable;
    std::unordered_map<std::string, bool> variableDeclared;
    std::unordered_map<std::string, bool> functionDeclared;

    bool inFunction = false;
    std::string currentFunction;

    for (size_t i = 0; i < tokens.size(); ++i)
    {
        const Token& token = tokens[i];

        if (token.content == "function" || token.content == "procedure")
        {
            inFunction = true;
            currentFunction = tokens[i + 1].content;
            functionDeclared[currentFunction] = true;
        }
        else if (token.content == "begin")
        {
            variableDeclared.clear();
        }
        else if (token.content == "end")
        {
            inFunction = false;
            currentFunction.clear();
        }
        else if (token.type == "identifier" && !isAKeyword(token.content))
        {
            if (!inFunction && symbolTable.find(token.content) == symbolTable.end() && variableDeclared.find(token.content) == variableDeclared.end() && !functionDeclared[token.content])
            {
                std::cout << "Erro: Variável ou função '" << token.content << "'" << "na linha " << token.line <<  " não declarada.\n";
            }
            else if (inFunction && variableDeclared.find(token.content) == variableDeclared.end())
            {
                if (symbolTable.find(token.content) == symbolTable.end() && !functionDeclared[token.content])
                {
                    std::cout << "Erro: Variável ou função na linha " << token.line << " '" << token.content << "' não declarada dentro da função '" << currentFunction << "'.\n";
                }
            }
        }
        else if (token.content == "var")
        {
            std::string variableName;

            // Find the variable name after "var" keyword
            while (i + 1 < tokens.size() && tokens[i + 1].type == "identifier")
            {
                variableName = tokens[i + 1].content;
                ++i;

                if (!isAKeyword(variableName) && symbolTable.find(variableName) == symbolTable.end() && variableDeclared.find(variableName) == variableDeclared.end())
                {
                    variableDeclared[variableName] = true;
                    symbolTable[variableName] = IdentifierType::VARIABLE;
                }
                else
                {
                    std::cout << "Erro: Declaração duplicada da variável '" << variableName << "'" << "encontrada na linha " << token.line << ".\n";
                }
            }
        }
    }
}
void generateSymbolTable(const std::vector<Token>& tokens)
{
    std::unordered_map<std::string, std::string> symbolTable;
    std::unordered_map<std::string, bool> variableDeclared;
    std::unordered_map<std::string, bool> functionDeclared;

    bool inFunction = false;
    std::string currentFunction;

    for (size_t i = 0; i < tokens.size(); ++i)
    {
        const Token& token = tokens[i];

        if (token.content == "function" || token.content == "procedure")
        {
            inFunction = true;
            currentFunction = tokens[i + 1].content;
            functionDeclared[currentFunction] = true;
        }
        else if (token.content == "begin")
        {
            variableDeclared.clear();
        }
        else if (token.content == "end")
        {
            inFunction = false;
            currentFunction.clear();
        }
       
        else if (token.content == "var")
        {
            std::string variableName;

            // Find the variable name after "var" keyword
            while (i + 1 < tokens.size() && tokens[i + 1].type == "identifier")
            {
                variableName = tokens[i + 1].content;
                ++i;

                if (!isAKeyword(variableName) && symbolTable.find(variableName) == symbolTable.end() && variableDeclared.find(variableName) == variableDeclared.end())
                {
                    variableDeclared[variableName] = true;
                    symbolTable[variableName] = currentFunction.empty() ? "global" : currentFunction;
                }
            }
        }
    }

    std::cout << "Tabela de Símbolos:\n";
    std::cout << "----------------------------\n";
    std::cout << "Lexema\t\tEscopo\n";
    std::cout << "----------------------------\n";

    for (const auto& entry : symbolTable)
    {
        std::cout << entry.first << "\t\t" << entry.second << "\n";
    }

    std::cout << "----------------------------\n";
}


int main()
{
    string fileName = "example-file.txt";
    vector<string> fileInput = getFileInput(fileName);

    std::string combinedInput;
    for (const auto &line : fileInput)
    {
        combinedInput += line + "\n"; // Preservando as quebras de linha
    }

    vector<Token> tokens = lexical_analysis(combinedInput);
    // for (const auto &token : tokens)
    // {
    //     cout << "Conteúdo: " << token.content << ", Tipo: " << token.type << endl;
    // }

    vector<pair<string, string>> pairs;
    for (const auto &token : tokens)
    {
        pairs.push_back(make_pair(token.content, token.type));
    }

    generateSymbolTable(tokens);
    checkTypeCompatibility(tokens);
    checkReservedIdentifierMisuse(tokens);
    checkFunctionExistence(pairs);
    checkParameterMismatch(tokens);
    computeUndeclaredVariables(tokens);
    checkFunctionAndValueType(tokens);

    return 0;
}
