#include <iostream>
#include "utils.hpp"
#include "GameBoard.hpp"
#include "io.hpp"
#include "search_algorithms.hpp"
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

    Game::GameBoard gmBoardChecker(currentPenguPosition, Game::Status::INITIAL);

    cout << gmBoard;

    gmBoard = search_algorithms::breadthFirstSearch(gmBoard);

    cout << gmBoard;

    for (int i = 0; i < gmBoard.path.size(); i++)
    {
        gmBoardChecker.path.push_back(gmBoard.path[i]);
        gmBoardChecker.simulateTraversingInTheSameDirection();
    }
    cout << gmBoardChecker;
    return 0;
}
