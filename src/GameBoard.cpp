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

map<string, vector<pair<int, int>>> createInitObjForMapOfSymbols()
{
    map<string, vector<pair<int, int>>> tempMapOfSymbols;
    for (const pair<char, string> eachSymbol : Game::symbolToNames)
    {

        tempMapOfSymbols.emplace(eachSymbol.second, vector<pair<int, int>>());
    }
    return tempMapOfSymbols;
}

string *Game::GameBoard::grid = nullptr;
int Game::GameBoard::gridRowSize = 0;
int Game::GameBoard::gridColSize = 0;
map<string, vector<pair<int, int>>> Game::GameBoard::symbolPositions = createInitObjForMapOfSymbols();
Game::GameBoard::GameBoard(){};
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
ostream &Game::GameBoard::printGrid(ostream &os)
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
pair<int, int> Game::GameBoard::getNewPosition(pair<int, int> currentPosition, int direction)
{
    pair<int, int> directionCoordinates = directions[direction];
    return make_pair(currentPosition.first + directionCoordinates.first, currentPosition.second + directionCoordinates.second);
}
bool Game::GameBoard::doesPositionHasGivenItem(pair<int, int> position, string symbolName)
{
    return Game::symbolToNames.at(grid[position.first][position.second]) == symbolName;
}
bool Game::GameBoard::checkAMoveIsInvalid(pair<int, int> position)
{
    auto isWall = [this](pair<int, int> position)
    {
        return doesPositionHasGivenItem(position, "wall");
    };
    auto isPositionOutOfRowExtremes = [this](pair<int, int> position)
    {
        return position.first > gridRowSize - 1 || position.first <= 0;
    };
    auto isPositionOutOfColumnExtremes = [this](pair<int, int> position)
    {
        return position.second > gridColSize - 1 || position.second <= 0;
    };
    vector<function<bool(pair<int, int>)>> functions;
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
vector<pair<int, int>> Game::GameBoard::getValidPositions(pair<int, int> currentPosition, int direction)
{
    vector<pair<int, int>> validPositions;
    for (int directionIndex = 0; directionIndex < Game::directionsCount; directionIndex++)
    {
        pair<int, int> eachDirection = directions[directionIndex];
        set<int> invalidDirections = {0, 5};
        if (invalidDirections.find(directionIndex) != invalidDirections.end())
            continue;
        pair<int, int> newPosition = getNewPosition(currentPosition, directionIndex);
        if (checkAMoveIsInvalid(newPosition))
            continue;
        validPositions.push_back(newPosition);
    }
    return validPositions;
}
string Game::GameBoard::getStateStringWithGivenKeys(pair<int, int> fromPosition, pair<int, int> toPosition, vector<pair<int, int>> fishesCaught)
{
    char *stateString;
    size_t size = snprintf(NULL, 0, "FR_%d_FC_%d_TR_%d_TC_%d", fromPosition.first, fromPosition.second, toPosition.first, toPosition.second);
    snprintf(stateString, size, "FR_%d_FC_%d_TR_%d_TC_%d", fromPosition.first, fromPosition.second, toPosition.first, toPosition.second);
    for (int i = 0; i < fishesCaught.size(); i++)
    {
        snprintf(stateString, strlen(stateString), "_FSH%d_R%d_C%d", i, fishesCaught[i].first, fishesCaught[i].second);
    }
    return string(stateString);
}
ostream &Game::operator<<(ostream &os, Game::GameBoard gameBoard)
{
    os << "from the operator overloading" << endl;
    gameBoard.printGrid(os);
    os << Game::GameBoard::gridRowSize << endl;
    os << Game::GameBoard::gridColSize << endl;
    return os;
}