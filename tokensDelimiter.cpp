#ifndef DELIMITERS_H
#define DELIMITERS_H

#include "tokensDelimiter.h"

struct Delimiter {
    char name;
    TokenDelimiter token;
};

// Definição dos delimitadores
Delimiter delimiters[] = {
    {'+', TokenDelimiter::PLUS},
    {'-', TokenDelimiter::MINUS},
    {'*', TokenDelimiter::TIMES},
    {'/', TokenDelimiter::DIVIDE},
    {',', TokenDelimiter::COMMA},
    {'#', TokenDelimiter::HASH},
    {'=', TokenDelimiter::EQUALS},
    {'>', TokenDelimiter::GREATER_THAN},
    {'<', TokenDelimiter::LESS_THAN},
    {'[', TokenDelimiter::LEFT_BRACKET},
    {']', TokenDelimiter::RIGHT_BRACKET},
    {'(', TokenDelimiter::LEFT_PARENTHESIS},
    {')', TokenDelimiter::RIGHT_PARENTHESIS},
    {':', TokenDelimiter::COLON},
    {';', TokenDelimiter::SEMICOLON},
    {'/', TokenDelimiter::DOUBLE_SLASH},
    {'#', TokenDelimiter::DOUBLE_HASH}
};

#endif