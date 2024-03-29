#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <regex>

using namespace std;

#include "../../bnf/keywords.h"
#include "../../bnf/operators.h"
#include "../../bnf/delimiters.h"
#include "../../bnf/types.h"

#include "./current-command-handler.cpp"

#define WAS_ENTIRE_COMMAND_VERIFIED -1

vector<Token> lexicalAnalyzerAutomata(vector<string> commands)
{
    vector<Token> allTokens;
    for (string command : commands)
    {
        bool isAnEmptyLine = command.length() == 0;
        if (isAnEmptyLine)
            continue;
        int character_stopped = 0;
        while (character_stopped != WAS_ENTIRE_COMMAND_VERIFIED)
        {
            Token token = handleCurrentCommandVerification(command, &character_stopped);
            allTokens.push_back(token);
            
            // gambiarra mas deu certo
            if (token.content.compare("stopVerification")) break;
        }
    }
    return allTokens;
}