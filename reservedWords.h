#ifndef RESERVED_WORDS_H
#define RESERVED_WORDS_H

#include <string>
#include "tokens.h"

struct ReservedWord {
    std::string name;  // nome da palavra reservada
    Token token;         // token associado à palavra
};

#endif