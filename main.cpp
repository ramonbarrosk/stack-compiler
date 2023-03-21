#include <iostream>
#include <vector>
#include <string>
#include <fstream>

// Functions
#include "lexer/lexer.cpp"

using namespace std;

int main()
{
  lexer("example-file.txt");
  return 0;
}
