#include<iostream>
#include<vector>
#include<string>
#ifndef IO_H
#define IO_H

namespace IO{
    std::vector<std::string> getDataFromFile(std::string filePath);
    std::pair<int,int> getGridSizeFromInputData(std::vector<std::string> lines);
    std::string* getGridFromInputData(std::vector<std::string> lines);
}
#endif