#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include "../tokens.h"
using namespace std;

// Função que verifica se um caractere é uma letra
bool is_letter(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

// Função que verifica se um caractere é um dígito
bool is_digit(const string& str)
{
    return !str.empty() && all_of(str.begin(), str.end(), [](char c) {
        return isdigit(static_cast<unsigned char>(c));
    });
}
// Função que verifica se um caractere é um espaço em branco
bool is_whitespace(char c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

// Função que verifica se um caractere é um símbolo
bool is_symbol(char c)
{
    static const string symbols = ";:,()[]{}+-*/<>=.#";
    return symbols.find(c) != string::npos;
}

// Função que verifica se um caractere é um operador
bool is_operator(char c)
{
    static const string operators = "+-*/<>=//";
    return operators.find(c) != string::npos;
}

// Função que verifica se um caractere é um delimitador
bool is_delimiter(char c)
{
    static const string delimiters = " ,;:()[]{}";
    return delimiters.find(c) != string::npos;
}

// Função que verifica se uma palavra é uma palavra-chave
Token is_keyword(const string &word)
{
    static const map<string, Token> keywords = {
        {"program", Token::PROGRAM},
        {"var", Token::VAR},
        {"integer", Token::INTEGER},
        {"real", Token::REAL},
        {"pilha", Token::PILHA_OF_INTEGER},
        {"of", Token::OF},
        {"procedure", Token::PROCEDURE},
        {"function", Token::FUNCTION},
        {"begin", Token::BEGIN},
        {"end", Token::END},
        {"read", Token::READ},
        {"write", Token::WRITE},
        {"for", Token::FOR},
        {"to", Token::TO},
        {"do", Token::DO},
        {"repeat", Token::REPEAT},
        {"until", Token::UNTIL},
        {"while", Token::WHILE},
        {"if", Token::IF},
        {"then", Token::THEN},
        {"else", Token::ELSE},
        {"input", Token::INPUT},
        {"output", Token::OUTPUT},
        {"length", Token::LENGTH},
    };
    return keywords.count(word) > 0 ? keywords.at(word) : Token::IDENTIFIER;
}

// Função que verifica se uma palavra é um identificador
bool is_identifier(const string &word)
{
    if (word.empty())
        return false;
    if (!is_letter(word[0]))
        return false;
    for (size_t i = 1; i < word.size(); i++)
    {
        if (!is_letter(word[i]) && !is_digit(word[i]))
        {
            return false;
        }
    }
    return true;
}

// Função que verifica se uma palavra é um número inteiro
bool is_integer(const string &word)
{
    if (word.empty())
        return false;
    for (char c : word)
    {
        if (!is_digit(c))
            return false;
    }
    return true;
}
