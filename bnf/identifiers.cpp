#include <string>
#include <vector>
#include <cstring>
#include <regex>

#include "identifiers.h"
#include "types.h"

using namespace std;

vector<char> identifierEndCharacters;

/**
 * @brief 
 * 
 * Com esses caracteres, o identificador é um nome de um método de uma classe
 */
void initializeEndCharactersVector() {
    identifierEndCharacters.push_back('_');
    for (char c = 'a'; c <= 'z'; c++) {
        identifierEndCharacters.push_back(c);
    }
    for (char c = 'A'; c <= 'Z'; c++) {
        identifierEndCharacters.push_back(c);
    }
    for (char c = '0'; c <= '9'; c++) {
        identifierEndCharacters.push_back(c);
    }
}


bool isAnIdentifierEndCharacter(char endCharacter) {
    if (identifierEndCharacters.size() == 0) {
        initializeEndCharactersVector();
    }
    for (int i = 0; i < identifierEndCharacters.size(); i++){
        if (identifierEndCharacters[i] == endCharacter) return true;
    }
    return false;
}