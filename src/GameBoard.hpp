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
        bool operator<(const Position &b) const
        {
            if (row - b.row != 0)
            {
                return row - b.row < 0;
            }
            return column - b.column < 0;
        }
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
        static void initGrid(std::string *grid, int gridRowSize, int gridColSize);
        static void initSymbolPositions();
        Position getNewPosition(Position currentPosition, int direction);
        bool checkAMoveIsInvalid(Position position);
        bool doesPositionHasGivenItem(Position position, std::string symbolName);
        bool isPenguKilled(Position position);
        std::vector<Position> getValidPositions(Position currentPosition, int direction);
        std::string getStateStringWithGivenKeys(Position fromPosition, Position toPosition, std::vector<Position> fishesCaught);
        void printGrid(std::ostream &os);
        void simulateTraversingInTheSameDirection(Game::GameBoard &currentState);
    };
    std::ostream &operator<<(std::ostream &os, Game::GameBoard gameBoard);
}
#endif