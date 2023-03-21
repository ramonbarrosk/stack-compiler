// main.cpp

// Libs
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

// Functions
#include "getFileInput.cpp"
#include "lexer/lexer.cpp"

using namespace std;

int main()
{
  string fileName = "example-file.txt";
  vector<string> fileInput = getFileInput(fileName);

  lexicalAnalyzer(fileInput);
  return 0;
}