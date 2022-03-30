#include<iostream>
#include"utils.hpp"
#include"GameBoard.hpp"
#include"io.hpp"
using namespace std;
using namespace utils;
void initGrid(string* grid, int gridRowSize, int gridColSize){
        Game::GameBoard::grid = new string[gridRowSize];
        for(int i=0;i<gridRowSize;i++){
           Game::GameBoard::grid[i] = grid[i]; 
        }
        Game::GameBoard::gridRowSize = gridRowSize;
        Game::GameBoard::gridColSize = gridColSize;
}
int main(){
    cout<<charToInt('8')<<endl;
    string grid[6] = {
        "#######",
        "#P# *0#",
        "#*0*U*#",
        "#0 S*##",
        "#***S*#",
        "#######"
    };
    Game::GameBoard gmBoard;
    initGrid(grid, 6, 7);
    pair<int,int> direction = gmBoard.getNewPosition(make_pair(4,3),1);
    cout<<direction.first<<direction.second<<endl;
    cout<<gmBoard<<endl;
    cout<<gmBoard.checkAMoveIsInvalid(make_pair(-1,-2))<<endl;
    vector<pair<int,int>> validMoves = gmBoard.getValidPositions(make_pair(1,1),1);
    cout<<validMoves.size()<<endl;
    return 0;
}
