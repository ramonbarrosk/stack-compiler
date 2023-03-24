#include <string>
#include <vector>
#include <cstring>
#include <regex>

#include "keywords.h"
#include "types.h"

using namespace std;

regex keywords ("program|var|integer|real|pilhaofinteger|pilhaofreal|procedure|function|begin|end|read|write|for|to|do|repeat|until|while|if|then|else|input|output|length|concatena|end");

bool isAKeyword(string word) {
  return !!regex_match(word, keywords);
}