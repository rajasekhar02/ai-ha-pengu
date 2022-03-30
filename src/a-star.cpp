#include <iostream>
#include "utils.hpp"
#include "GameBoard.hpp"
#include "io.hpp"
using namespace std;
using namespace utils;

int main()
{
    vector<string> lines = IO::getDataFromFile("./input/input.txt");
    pair<int, int> gridSize = IO::getGridSizeFromInputData(lines);
    string *grid = IO::getGridFromInputData(lines);
    Game::GameBoard::initGrid(grid, gridSize.first, gridSize.second);
    Game::GameBoard gmBoard;
    pair<int, int> direction = gmBoard.getNewPosition(make_pair(4, 3), 1);
    cout << direction.first << direction.second << endl;
    cout << gmBoard << endl;
    cout << gmBoard.checkAMoveIsInvalid(make_pair(-1, -2)) << endl;
    vector<pair<int, int>> validMoves = gmBoard.getValidPositions(make_pair(1, 1), 1);
    cout << validMoves.size() << endl;
    return 0;
}
