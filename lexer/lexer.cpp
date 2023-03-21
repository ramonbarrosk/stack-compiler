#include <string>
#include <vector>
#include <cstring>
#include <regex>

#include "lexer.h"

#include "./utils/withdraw-spaces.cpp"
#include "./utils/split-commands-by-delimiter.cpp"
#include "./utils/eat-lines.cpp"

using namespace std;

void lexicalAnalyzer(vector<string> script) {
    vector<string> commands = eat_first_blanks(script);
    commands = withdraw_spaces(commands);
    commands = splitCommandsByDelimiter(commands, ' ');
    // retornar um vetor de tokens
    for (int j = 0; j < commands.size(); j++)
    {
      for (int k = 0; k < commands[j].size(); k++)
      {
        cout << commands[j][k];
      }
    cout << '\n';
    }
}