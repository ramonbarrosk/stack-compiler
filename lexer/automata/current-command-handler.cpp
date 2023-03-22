#include <string>
#include <vector>
#include <cstring>
#include <stdexcept>

#include "./alphanumeric-handler.cpp"
#include "./number-handler.cpp"
#include "./operator-handler.cpp"
#include "./delimiter-handler.cpp"

#include "../../bnf/types.h"

using namespace std;

Token handleCurrentCommandVerification(string command, int *characterStopped)
{
    Token newToken;
    newToken.content = "";
    newToken.type = IDENTIFIER;

    string currentCharacter (1, command[*characterStopped]);
    if (isalpha(command[*characterStopped]))
    {
        return handleAlphanumericThatCanBeAnIdentifierOrKeyword(command, characterStopped);
    }
    if (isdigit(command[*characterStopped]))
    {
        return handleNumbersAndThrowIfThereIsALetter(command, characterStopped);
    }
    if (isAnOperator(currentCharacter)) {
        return handleOperators(command, characterStopped);
    }
    if (isADelimiter(currentCharacter)) {
        return handleDelimitersThatCanBeOperatorOrDelimiter(command, characterStopped);
    }

    return newToken;
}