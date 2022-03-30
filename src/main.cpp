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

    Game::GameBoard::initSymbolPositions();

    Game::Position currentPenguPosition = Game::GameBoard::getPenguPositionFromInput();

    Game::GameBoard::clearPenguPositionOnGrid();

    Game::GameBoard gmBoard(currentPenguPosition, Game::Status::INITIAL);

    cout << gmBoard;

    int directionToTraverse;

    cin >> directionToTraverse;

    if (directionToTraverse < Game::directionsCount && directionToTraverse >= 0)
    {
        gmBoard.path.push_back(directionToTraverse);

        gmBoard.simulateTraversingInTheSameDirection();

        cout << gmBoard;
    }

    return 0;
}
