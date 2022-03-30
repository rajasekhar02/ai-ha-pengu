#include<fstream>
#include"utils.hpp"
#include"io.hpp"
using namespace std;
vector<string> IO::getDataFromFile(string filePath){
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
std::pair<int,int> IO::getGridSizeFromInputData(std::vector<string> lines){
    vector<string> gridSize = utils::split(lines[0],' ');
    return make_pair(stoi(utils::trim(gridSize[0])),stoi(utils::trim(gridSize[1])));
}
std::string* IO::getGridFromInputData(std::vector<string> lines){
    vector<string> grid(lines.begin()+1,lines.end());
    string* gridStringArr = new string[grid.size()];
    for(int i=0;i<grid.size();i++){
        gridStringArr[i] = utils::trim(grid[i]);
    }
    return gridStringArr;
}