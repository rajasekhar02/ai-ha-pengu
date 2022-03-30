#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <cstring>
#include "utils.hpp"

#ifndef GAME_BOARD_H
#define GAME_BOARD_H

namespace Game
{
    const std::map<char, std::string> symbolToNames = {
        {'*', "fish"},
        {'0', "snow"},
        {'S', "shark"},
        {'U', "bear"},
        {'#', "wall"},
        {'P', "pengu"},
        {' ', "ice"}};
    const int directionsCount = 10;
    const std::pair<int, int> directions[directionsCount] = {
        std::make_pair(0, 0),
        std::make_pair(1, -1),
        std::make_pair(1, 0),
        std::make_pair(1, 1),
        std::make_pair(0, -1),
        std::make_pair(0, 0),
        std::make_pair(0, 1),
        std::make_pair(-1, -1),
        std::make_pair(-1, 0),
        std::make_pair(-1, 1)};
    class GameBoard
    {
    public:
        static std::string *grid;
        static int gridRowSize;
        static int gridColSize;
        static std::map<std::string, std::vector<std::pair<int, int>>> symbolPositions;
        std::pair<int, int> currentPenguPosition;
        GameBoard();
        static void initGrid(std::string *grid, int gridRowSize, int gridColSize);
        std::pair<int, int> getNewPosition(std::pair<int, int> currentPosition, int direction);
        bool checkAMoveIsInvalid(std::pair<int, int> position);
        bool doesPositionHasGivenItem(std::pair<int, int> position, std::string symbolName);
        std::vector<std::pair<int, int>> getValidPositions(std::pair<int, int> currentPosition, int direction);
        std::string getStateStringWithGivenKeys(std::pair<int, int> fromPosition, std::pair<int, int> toPosition, std::vector<std::pair<int, int>> fishesCaught);
        std::ostream &printGrid(std::ostream &os);
    };
    std::ostream &operator<<(std::ostream &os, Game::GameBoard gameBoard);
}
#endif