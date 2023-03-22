#include <string>
#include <vector>
#include <cstring>
#include <regex>

#include "lexer.h"

#include "./utils/withdraw-spaces.cpp"
#include "./utils/split-commands-by-delimiter.cpp"
#include "./utils/eat-lines.cpp"

#include "./automata/main.cpp"

using namespace std;

vector<Token> lexicalAnalyzer(vector<string> script) {
    vector<string> commands = eat_first_blanks(script);
    commands = withdraw_spaces(commands);
    commands = splitCommandsByDelimiter(commands, ' ');
    // criar automato ainda pqp (é necessário?);
    return lexicalAnalyzerAutomata(commands);
}