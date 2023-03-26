// main.cpp

// Libs
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

// Functions
#include "getFileInput.cpp"
#include "lexer/lexer.cpp"
#include "./syntactic/syntactic-analyzer.cpp"

using namespace std;

int main()
{
  string fileName = "example-file.txt";
  vector<string> fileInput = getFileInput(fileName);

  vector<Token> tokens = lexicalAnalyzer(fileInput);
    // for (Token token : tokens) {
    //     if (token.content.compare("\n") == 0) {
    //         cout << token.type << ": " << endl;
    //     } else {
    //         cout << token.content << " : " << token.type << endl;
    //     }
    // }
  cout << "\n";
  syntacticAnalyzer(tokens);
  return 0;
}