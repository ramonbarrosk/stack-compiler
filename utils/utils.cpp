#include <iostream>
#include <fstream>
#include <string>
#include <map>
using namespace std;

// Função que verifica se um caractere é uma letra
bool is_letter(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

// Função que verifica se um caractere é um dígito
bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

// Função que verifica se um caractere é um espaço em branco
bool is_whitespace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

// Função que verifica se um caractere é um símbolo
bool is_symbol(char c) {
    static const string symbols = ";:,()[]{}+-*/<>=.#";
    return symbols.find(c) != string::npos;
}

// Função que verifica se um caractere é um operador
bool is_operator(char c) {
    static const string operators = "+-*/<>=//";
    return operators.find(c) != string::npos;
}

// Função que verifica se um caractere é um delimitador
bool is_delimiter(char c) {
    static const string delimiters = " ,;:()[]{}";
    return delimiters.find(c) != string::npos;
}

// Função que verifica se uma palavra é uma palavra-chave
bool is_keyword(const string& word) {
    static const map<string, bool> keywords = {
        {"program", true},
        {"var", true},
        {"integer", true},
        {"real", true},
        {"pilha", true},
        {"of", true},
        {"procedure", true},
        {"function", true},
        {"begin", true},
        {"end", true},
        {"read", true},
        {"write", true},
        {"for", true},
        {"to", true},
        {"do", true},
        {"repeat", true},
        {"until", true},
        {"while", true},
        {"if", true},
        {"then", true},
        {"else", true},
        {"input", true},
        {"output", true},
        {"length", true}
    };
    return keywords.count(word) > 0;
}

// Função que verifica se uma palavra é um identificador
bool is_identifier(const string& word) {
    if (word.empty()) return false;
    if (!is_letter(word[0])) return false;
    for (size_t i = 1; i < word.size(); i++) {
        if (!is_letter(word[i]) && !is_digit(word[i])) {
            return false;
        }
    }
    return true;
}

// Função que verifica se uma palavra é um número inteiro
bool is_integer(const string& word) {
    if (word.empty()) return false;
    for (char c : word) {
        if (!is_digit(c)) return false;
    }
    return true;
}
