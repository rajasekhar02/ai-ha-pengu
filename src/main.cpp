#include <iostream>
#include "utils.hpp"
#include "GameBoard.hpp"
#include "io.hpp"
#include "search_algorithms.hpp"
using namespace std;
using namespace utils;

int main(int argc, char **argv)
{
    vector<string> lines = IO::getDataFromFile(string(argv[1]));

    pair<int, int> gridSize = IO::getGridSizeFromInputData(lines);

    string *grid = IO::getGridFromInputData(lines);

    Game::GameBoard::initGrid(grid, gridSize.first, gridSize.second);

    Game::GameBoard::initSymbolPositions();

    Game::Position currentPenguPosition = Game::GameBoard::getPenguPositionFromInput();

    Game::GameBoard::clearPenguPositionOnGrid();

    Game::GameBoard gmBoard(currentPenguPosition, Game::Status::INITIAL);

    gmBoard = search_algorithms::depthFirstSearch(gmBoard);

    cout << gmBoard;

    Game::GameBoard gmBoardChecker(currentPenguPosition, Game::Status::INITIAL);

    for (int i = 0; i < gmBoard.path.size(); i++)
    {
        gmBoardChecker.path.push_back(gmBoard.path[i]);
        gmBoardChecker.simulateTraversingInTheSameDirection();
    }

    cout << gmBoardChecker;

    cout << ((gmBoard.fishesCaughtWhileTraversing.size() == gmBoardChecker.fishesCaughtWhileTraversing.size()) ? "Accepted" : "Wrong Answer") << endl;

    return 0;
}
