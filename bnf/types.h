#include <string>
#include <cstring>

#define KEYWORD "KEYWORD"
#define IDENTIFIER "IDENTIFIER"
#define OPERATOR "OPERATOR"
#define DELIMITER "DELIMITER"
#define NUMBER "NUMBER"

#pragma once

using namespace std;

typedef struct aux {
    std::string content;
    std::string type;
    std::string scope;
    std::string value;  // Novo campo para armazenar o valor (se disponível)
    int line;  // Linha em que o token aparece
} Token;