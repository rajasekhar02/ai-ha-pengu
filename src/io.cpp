#include<fstream>
#include"io.hpp"
using namespace std;
static vector<string> IO::getDateFromFile(string filePath){
       ifstream input(filePath);
       vector<string> lines;
        if(!input.is_open()) return {""};
        while(!input.eof()){
            string line;
            getline(input,line);
            lines.push_back(line);
        }
       return lines;
}