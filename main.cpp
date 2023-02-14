// main.cpp

// Libs
#include <iostream>
#include <vector>
#include <string>
#include <fstream>

// Functions
#include "getFileInput.h"

using namespace std;

int main()
{
  // TODO: Get the fileName from the user in string format, following the standard: "name-of-file.txt"
  string fileName = "example-file.txt";
  vector<string> fileInput = getFileInput(fileName);

  // TODO: Remove this example loop below:
  for (int i = 0; i < fileInput.size(); i++)
  {
    cout << i << " " << fileInput[i] << '\n';
  }

  // TODO: Remove this example code below to print out individual letters:
  /*
  cout << "Printing out all individual letters: \n";
  for (int j = 0; j < fileInput.size(); j++)
  {
    for (int k = 0; k < fileInput[j].size(); k++)
    {
      cout << fileInput[j][k] << ' ';
    }
    cout << '\n';
  }
  */

  return 0;
}
