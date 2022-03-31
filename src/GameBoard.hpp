#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <cstring>
#include "utils.hpp"

#ifndef GAME_BOARD_H
#define GAME_BOARD_H

namespace Game
{
    enum Status
    {
        INITIAL,
        VICTORY,
        FAILED,
        KILLED,
        ALIVE,
        ON_SNOW,
        STRUCT_BY_WALL
    };
    const std::map<char, std::string> symbolToNames = {
        {'*', "fish"},
        {'0', "snow"},
        {'S', "shark"},
        {'U', "bear"},
        {'#', "wall"},
        {'P', "pengu"},
        {' ', "ice"}};
    const int directionsCount = 10;
    struct Position
    {
        int row;
        int column;
        Position();
        Position(int row, int column);
        bool operator<(const Position &b) const;
        bool operator==(const Position &b) const;
    };
    const Position directions[directionsCount] = {
        {0, 0},
        {1, -1},
        {1, 0},
        {1, 1},
        {0, -1},
        {0, 0},
        {0, 1},
        {-1, -1},
        {-1, 0},
        {-1, 1}};
    const std::string symbolsToCollectPositions[2] = {"fish", "pengu"};
    struct GameBoard
    {
        static std::string *grid;
        static int gridRowSize;
        static int gridColSize;
        static std::map<std::string, std::vector<Position>> symbolPositions;
        Position currentPenguPosition;
        Status status;
        std::set<Position> fishesCaughtWhileTraversing;
        std::vector<int> path;
        GameBoard();
        GameBoard(Position currentPenguPosition, Status status);
        GameBoard clone();
        static void initGrid(std::string *grid, int gridRowSize, int gridColSize);
        static void initSymbolPositions();
        static Game::Position getPenguPositionFromInput();
        static int getTotalFishCount();
        static void clearPenguPositionOnGrid();
        Position getNewPosition(Position currentPosition, int direction);
        bool checkAMoveIsInvalid(Position position);
        bool doesPositionHasGivenItem(Position position, std::string symbolName);
        bool isPenguKilled(Position position);
        Position *getValidPositions();
        std::string getStateStringWithGivenKeys(Position fromPosition, Position toPosition, std::set<Position> fishesCaught);
        void printGrid(std::ostream &os);
        void simulateTraversingInTheSameDirection();
    };
    std::ostream &operator<<(std::ostream &os, Game::GameBoard gameBoard);
}
#endif