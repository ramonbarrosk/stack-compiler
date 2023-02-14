// getFileInput.cpp
#include "getFileInput.h"

std::vector<std::string> getFileInput(std::string fileName)
{
    std::ifstream file(fileName);
    std::string str;

    std::vector<std::string> stringArr;

    while (std::getline(file, str))
    {
        stringArr.push_back(str);
    }

    return stringArr;
}