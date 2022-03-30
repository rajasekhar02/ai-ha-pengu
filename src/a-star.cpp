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
    Game::Position direction = gmBoard.getNewPosition({4, 3}, 1);
    cout << direction.row << direction.column << endl;
    cout << gmBoard << endl;
    cout << gmBoard.checkAMoveIsInvalid({-1, -2}) << endl;
    vector<Game::Position> validMoves = gmBoard.getValidPositions({1, 1}, 1);
    cout << validMoves.size() << endl;
    return 0;
}
