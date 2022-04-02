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
Game::GameBoard::GameBoard() {}
Game::GameBoard::GameBoard(Game::Position currentPenguPosition, Status status)
{
    this->status = status;
    this->currentPenguPosition = currentPenguPosition;
};
Game::GameBoard Game::GameBoard::clone()
{
    const Game::GameBoard *gameBoard = this;
    Game::GameBoard gameBoardCopy;
    gameBoardCopy.currentPenguPosition.row = gameBoard->currentPenguPosition.row;
    gameBoardCopy.currentPenguPosition.column = gameBoard->currentPenguPosition.column;
    for (const Position fishPosition : gameBoard->fishesCaughtWhileTraversing)
    {
        gameBoardCopy.fishesCaughtWhileTraversing.emplace(fishPosition.row, fishPosition.column);
    }
    for (const int direction : gameBoard->path)
    {
        gameBoardCopy.path.push_back(direction);
    }
    gameBoardCopy.status = gameBoard->status;
    return gameBoardCopy;
}
Game::Position Game::GameBoard::getPenguPositionFromInput()
{
    return Game::GameBoard::symbolPositions.at("pengu")[0];
}
int Game::GameBoard::getTotalFishCount()
{
    return (int)Game::GameBoard::symbolPositions.at("fish").size();
}
void Game::GameBoard::clearPenguPositionOnGrid()
{
    Game::Position penguPosition = getPenguPositionFromInput();
    Game::GameBoard::grid[penguPosition.row][penguPosition.column] = ' ';
}
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
    for (int i = 0; i < Game::GameBoard::gridRowSize; i++)
    {
        for (int j = 0; j < Game::GameBoard::gridColSize; j++)
        {
            string symbolName = Game::symbolToNames.at(grid[i][j]);
            if (symbolName == "fish")
            {
                Game::GameBoard::symbolPositions.at(symbolName).push_back({i, j});
            }
            else if (symbolName == "pengu")
            {
                Game::GameBoard::symbolPositions.at(symbolName).push_back({i, j});
            }
        }
    }
}
std::string Game::GameBoard::castGameBoardToString()
{
    string st = "#0#";
    Game::GameBoard gM = *this;
    for (const Position p : gM.fishesCaughtWhileTraversing)
    {
        st += string("_") + to_string(p.row) + string("_") + to_string(p.column);
    }
    st += string("|#1#");
    for (const int direction : gM.path)
    {
        st += string("_") + to_string(direction);
    }
    st += string("|#2#_");
    st += to_string(gM.status);
    st += string("|#3#_");
    st += to_string(gM.currentPenguPosition.row) + string("_") + to_string(gM.currentPenguPosition.column);
    return st;
}
Game::GameBoard Game::GameBoard::castStringToGameBoard(string gameBoardString)
{
    Game::GameBoard gm;
    vector<string> gMProperties = utils::split(gameBoardString, '|');
    for (int i = 0; i < gMProperties.size(); i++)
    {
        vector<string> eachProperty = utils::split(gMProperties[i], '_');
        switch ((eachProperty[0][1] - '0'))
        {
        case 0:
            for (int i = 1; i < eachProperty.size(); i += 2)
            {
                gm.fishesCaughtWhileTraversing.emplace(stoi(eachProperty[i]), stoi(eachProperty[i + 1]));
            }
            break;
        case 1:
            for (int i = 1; i < eachProperty.size(); i += 1)
            {
                gm.path.push_back(stoi(eachProperty[i]));
            }
            break;
        case 2:
            gm.status = static_cast<Game::Status>(stoi(eachProperty[1]));
            break;
        case 3:
            gm.currentPenguPosition = {stoi(eachProperty[1]), stoi(eachProperty[2])};
            break;
        }
    }
    return gm;
}
void Game::GameBoard::simulateTraversingInTheSameDirection()
{
    Game::GameBoard *currentState = this;
    Game::Position currentPenguPosition = currentState->currentPenguPosition;
    set<Game::Position> fishesCaughtWhileTraversing = currentState->fishesCaughtWhileTraversing;
    vector<int> path = currentState->path;
    const int currentMovingDirectionIndex = path[path.size() - 1];

    Game::Position newPosition = getNewPosition(currentPenguPosition, currentMovingDirectionIndex);
    if (newPosition == currentPenguPosition)
        return;
    while (1)
    {
        if (doesPositionHasGivenItem(newPosition, "wall"))
        {

            break;
        }

        currentPenguPosition = newPosition;
        currentState->currentPenguPosition.row = currentPenguPosition.row;
        currentState->currentPenguPosition.column = currentPenguPosition.column;

        if (doesPositionHasGivenItem(newPosition, "snow"))
        {
            currentState->status = Game::Status::ON_SNOW;
            break;
        }
        else if (doesPositionHasGivenItem(newPosition, "fish") && currentState->fishesCaughtWhileTraversing.find(newPosition) == currentState->fishesCaughtWhileTraversing.end())
        {
            currentState->fishesCaughtWhileTraversing.insert(newPosition);
        }
        else if (isPenguKilled(newPosition))
        {

            currentState->status = Game::Status::KILLED;
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
Game::Position *Game::GameBoard::getValidPositions()
{
    Game::Position currentPosition = this->currentPenguPosition;
    Game::Position *validPositions = new Position[10];
    for (int directionIndex = 0; directionIndex < Game::directionsCount; directionIndex++)
    {
        Game::Position eachDirection = directions[directionIndex];
        set<int> invalidDirections = {0, 5};
        if (invalidDirections.find(directionIndex) != invalidDirections.end())
            continue;
        Game::Position newPosition = getNewPosition(currentPosition, directionIndex);
        if (checkAMoveIsInvalid(newPosition))
            continue;
        validPositions[directionIndex] = newPosition;
    }
    return validPositions;
}
string Game::GameBoard::getStateStringWithGivenKeys(Game::Position fromPosition, Game::Position toPosition, set<Game::Position> fishesCaught)
{
    vector<string> arrVisitedStrings;
    char *stateString;
    size_t size = snprintf(NULL, 0, "FR_%d_FC_%d_TR_%d_TC_%d", fromPosition.row, fromPosition.column, toPosition.row, toPosition.column);
    stateString = (char *)malloc(size + 1);
    snprintf(stateString, size + 1, "FR_%d_FC_%d_TR_%d_TC_%d", fromPosition.row, fromPosition.column, toPosition.row, toPosition.column);
    int i = 0;
    arrVisitedStrings.push_back(string(stateString));
    for (const Position fishPosition : fishesCaught)
    {
        free(stateString);
        size = snprintf(NULL, 0, "_FSH%d_R%d_C%d", i, fishPosition.row, fishPosition.column);
        stateString = (char *)malloc(size + 1);
        snprintf(stateString, size + 1, "_FSH%d_R%d_C%d", i, fishPosition.row, fishPosition.column);
        arrVisitedStrings.push_back(string(stateString));
        i++;
    }
    free(stateString);
    string state = "";
    for (const string st : arrVisitedStrings)
    {
        state += st;
    }
    return state;
}
void Game::GameBoard::printGrid(ostream &os)
{
    for (int i = 0; i < gridRowSize; i++)
    {
        for (int j = 0; j < gridColSize; j++)
        {
            Game::Position position = {i, j};
            if (position == this->currentPenguPosition)
            {
                os << (this->status == Game::Status::KILLED ? 'X' : 'P');
            }
            else if (this->fishesCaughtWhileTraversing.find({i, j}) != this->fishesCaughtWhileTraversing.end())
            {
                os << " ";
            }
            else
            {

                os << grid[i][j];
            }
        }
        os << endl;
    }
}
ostream &Game::operator<<(ostream &os, Game::GameBoard gameBoard)
{
    os << "Grid Size: " << Game::GameBoard::gridRowSize << " " << Game::GameBoard::gridColSize << endl;
    Game::Position penguInputPosition = Game::GameBoard::getPenguPositionFromInput();
    os << "Pengu Position From Input: " << penguInputPosition.row << " " << penguInputPosition.column << endl;
    os << "Current Pengu Position: " << gameBoard.currentPenguPosition.row << " " << gameBoard.currentPenguPosition.column << endl;
    os << "Fishes Capture Count: " << gameBoard.fishesCaughtWhileTraversing.size() << endl;
    os << "Path: ";
    for (int i = 0; i < gameBoard.path.size(); i++)
    {
        os << gameBoard.path[i];
    }
    os << endl;
    // gameBoard.printGrid(os);
    return os;
}

/************Position Struct Definitions******************/
Game::Position::Position()
{
    row = -1;
    column = -1;
}
Game::Position::Position(int row, int column)
{
    this->row = row;
    this->column = column;
}
bool Game::Position::operator<(const Position &b) const
{
    if (row - b.row != 0)
    {
        return row - b.row < 0;
    }
    return column - b.column < 0;
}
bool Game::Position::operator==(const Position &b) const
{
    return row == b.row && column == b.column;
}