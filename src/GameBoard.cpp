#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <set>
#include <cstring>
#include <functional>
#include "utils.hpp"
#include "GameBoard.hpp"

using namespace std;

map<string, vector<Game::Position>> createInitObjForMapOfSymbols()
{
    map<string, vector<Game::Position>> tempMapOfSymbols;
    for (const pair<char, string> eachSymbol : Game::symbolToNames)
    {
        if (Game::symbolsToCollectPositions[0] != eachSymbol.second && Game::symbolsToCollectPositions[1] != eachSymbol.second)
            continue;
        tempMapOfSymbols.emplace(eachSymbol.second, vector<Game::Position>());
    }
    return tempMapOfSymbols;
}

string *Game::GameBoard::grid = nullptr;
int Game::GameBoard::gridRowSize = 0;
int Game::GameBoard::gridColSize = 0;
map<string, vector<Game::Position>> Game::GameBoard::symbolPositions = createInitObjForMapOfSymbols();
Game::GameBoard::GameBoard()
{
    status = Game::Status::INITIAL;
};
void Game::GameBoard::initGrid(string *grid, int gridRowSize, int gridColSize)
{
    Game::GameBoard::grid = new string[gridRowSize];
    for (int i = 0; i < gridRowSize; i++)
    {
        Game::GameBoard::grid[i] = grid[i];
    }
    Game::GameBoard::gridRowSize = gridRowSize;
    Game::GameBoard::gridColSize = gridColSize;
}
void Game::GameBoard::initSymbolPositions()
{
}
void Game::GameBoard::simulateTraversingInTheSameDirection(Game::GameBoard &currentState)
{
    Game::Position currentPenguPosition = currentState.currentPenguPosition;
    set<Game::Position> fishesCaughtWhileTraversing = currentState.fishesCaughtWhileTraversing;
    vector<int> path = currentState.path;
    const int currentMovingDirectionIndex = path[path.size() - 1];
    Game::Position newPosition = getNewPosition(currentPenguPosition, currentMovingDirectionIndex);
    while (1)
    {
        if (doesPositionHasGivenItem(newPosition, "wall"))
        {
            break;
        }
        currentPenguPosition = newPosition;
        currentState.currentPenguPosition = currentPenguPosition;
        if (doesPositionHasGivenItem(newPosition, "snow"))
        {
            currentState.status = Game::Status::ON_SNOW;
            break;
        }
        if (doesPositionHasGivenItem(newPosition, "fish"))
        {
            currentState.fishesCaughtWhileTraversing.insert(newPosition);
        }
        if (isPenguKilled(newPosition))
        {
            currentState.status = Game::Status::KILLED;
            break;
        }
        newPosition = getNewPosition(currentPenguPosition, currentMovingDirectionIndex);
    }
}
Game::Position Game::GameBoard::getNewPosition(Game::Position currentPosition, int direction)
{
    Game::Position directionCoordinates = directions[direction];
    return {currentPosition.row + directionCoordinates.row, currentPosition.column + directionCoordinates.column};
}
bool Game::GameBoard::doesPositionHasGivenItem(Game::Position position, string symbolName)
{
    return Game::symbolToNames.at(grid[position.row][position.column]) == symbolName;
}
bool Game::GameBoard::checkAMoveIsInvalid(Game::Position position)
{
    auto isWall = [this](Game::Position position)
    {
        return doesPositionHasGivenItem(position, "wall");
    };
    auto isPositionOutOfRowExtremes = [this](Game::Position position)
    {
        return position.row > gridRowSize - 1 || position.row <= 0;
    };
    auto isPositionOutOfColumnExtremes = [this](Game::Position position)
    {
        return position.column > gridColSize - 1 || position.column <= 0;
    };
    vector<function<bool(Game::Position)>> functions;
    functions.push_back(isPositionOutOfRowExtremes);
    functions.push_back(isPositionOutOfColumnExtremes);
    functions.push_back(isWall);
    for (int eachCondition = 0; eachCondition < functions.size(); eachCondition++)
    {
        if (functions[eachCondition](position))
        {
            return true;
        }
    }
    return false;
}
bool Game::GameBoard::isPenguKilled(Game::Position position)
{
    return (doesPositionHasGivenItem(position, "bear") || doesPositionHasGivenItem(position, "shark"));
}
vector<Game::Position> Game::GameBoard::getValidPositions(Game::Position currentPosition, int direction)
{
    vector<Game::Position> validPositions;
    for (int directionIndex = 0; directionIndex < Game::directionsCount; directionIndex++)
    {
        Game::Position eachDirection = directions[directionIndex];
        set<int> invalidDirections = {0, 5};
        if (invalidDirections.find(directionIndex) != invalidDirections.end())
            continue;
        Game::Position newPosition = getNewPosition(currentPosition, directionIndex);
        if (checkAMoveIsInvalid(newPosition))
            continue;
        validPositions.push_back(newPosition);
    }
    return validPositions;
}
string Game::GameBoard::getStateStringWithGivenKeys(Game::Position fromPosition, Game::Position toPosition, vector<Game::Position> fishesCaught)
{
    char *stateString;
    size_t size = snprintf(NULL, 0, "FR_%d_FC_%d_TR_%d_TC_%d", fromPosition.row, fromPosition.column, toPosition.row, toPosition.column);
    snprintf(stateString, size, "FR_%d_FC_%d_TR_%d_TC_%d", fromPosition.row, fromPosition.column, toPosition.row, toPosition.column);
    for (int i = 0; i < fishesCaught.size(); i++)
    {
        snprintf(stateString, strlen(stateString), "_FSH%d_R%d_C%d", i, fishesCaught[i].row, fishesCaught[i].column);
    }
    return string(stateString);
}
void Game::GameBoard::printGrid(ostream &os)
{
    for (int i = 0; i < gridRowSize; i++)
    {
        for (int j = 0; j < gridColSize; j++)
        {
            os << grid[i][j];
        }
        os << endl;
    }
}
ostream &Game::operator<<(ostream &os, Game::GameBoard gameBoard)
{
    os << "from the operator overloading" << endl;
    gameBoard.printGrid(os);
    os << Game::GameBoard::gridRowSize << endl;
    os << Game::GameBoard::gridColSize << endl;
    return os;
}
