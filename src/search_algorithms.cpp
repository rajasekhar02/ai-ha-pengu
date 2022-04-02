#include "GameBoard.hpp"
#include "search_algorithms.hpp"
#include <deque>
#include <vector>
#include <iostream>
#include <set>
#include "search_algorithms_utils.hpp"
using namespace std;
namespace search_algorithms
{
    Game::GameBoard breadthFirstSearch(Game::GameBoard initGameBoard)
    {
        std::deque<Game::GameBoard> frontier;
        const int totalFishCount = Game::GameBoard::getTotalFishCount();
        set<string> visitedPaths;
        Game::GameBoard currGameBoard = initGameBoard;
        frontier.push_back(initGameBoard);
        while (!frontier.empty())
        {
            currGameBoard = frontier.front();
            frontier.pop_front();
            if (currGameBoard.fishesCaughtWhileTraversing.size() >= 20)
            {
                return currGameBoard;
            }
            if (currGameBoard.status == Game::Status::KILLED)
            {
                continue;
            }

            Game::Position *validPositions = currGameBoard.getValidPositions();
            for (int eachDirection = 0; eachDirection < Game::directionsCount; eachDirection++)
            {
                Game::Position position = validPositions[eachDirection];
                if (position.row == -1 && position.column == -1)
                {
                    continue;
                }
                Game::GameBoard newBoard = currGameBoard.clone();
                newBoard.path.push_back(eachDirection);
                newBoard.simulateTraversingInTheSameDirection();
                string stateString = newBoard.getStateStringWithGivenKeys(currGameBoard.currentPenguPosition, newBoard.currentPenguPosition, newBoard.fishesCaughtWhileTraversing);
                if (visitedPaths.find(stateString) == visitedPaths.end())
                {
                    visitedPaths.insert(stateString);
                    frontier.push_back(newBoard);
                }
            }
        }
        return currGameBoard;
    }
    Game::GameBoard depthFirstSearch(Game::GameBoard initGameBoard)
    {
        std::deque<Game::GameBoard> frontier;
        const int totalFishCount = Game::GameBoard::getTotalFishCount();
        set<string> visitedPaths;
        Game::GameBoard currGameBoard = initGameBoard;
        frontier.push_back(initGameBoard);
        while (!frontier.empty())
        {
            currGameBoard = frontier.back();
            frontier.pop_back();
            if (currGameBoard.fishesCaughtWhileTraversing.size() >= totalFishCount)
            {
                return currGameBoard;
            }
            if (currGameBoard.status == Game::Status::KILLED)
            {
                continue;
            }

            Game::Position *validPositions = currGameBoard.getValidPositions();
            for (int eachDirection = 0; eachDirection < Game::directionsCount; eachDirection++)
            {
                Game::Position position = validPositions[eachDirection];
                if (position.row == -1 && position.column == -1)
                {
                    continue;
                }
                Game::GameBoard newBoard = currGameBoard.clone();
                newBoard.path.push_back(eachDirection);
                newBoard.simulateTraversingInTheSameDirection();
                string stateString = newBoard.getStateStringWithGivenKeys(currGameBoard.currentPenguPosition, newBoard.currentPenguPosition, newBoard.fishesCaughtWhileTraversing);
                if (visitedPaths.find(stateString) == visitedPaths.end())
                {
                    visitedPaths.insert(stateString);
                    frontier.push_back(newBoard);
                }
            }
        }
        return currGameBoard;
    }
    Game::GameBoard parallelDepthFirshSearch(Game::GameBoard initGameBoard)
    {
        std::deque<Game::GameBoard> frontier;
        const int totalFishCount = Game::GameBoard::getTotalFishCount();
        set<string> visitedPaths;
        Game::GameBoard currGameBoard = initGameBoard;
        frontier.push_back(initGameBoard);
        while (!frontier.empty())
        {
            currGameBoard = frontier.back();
            frontier.pop_back();
            if (search_algorithm_utils::check_found_solution(currGameBoard))
            {
                return currGameBoard;
            }
            if (currGameBoard.status == Game::Status::KILLED)
            {
                continue;
            }

            Game::Position *validPositions = currGameBoard.getValidPositions();
            for (int eachDirection = 0; eachDirection < (Game::directionsCount); eachDirection++)
            {
                int directionToMove = eachDirection;
                Game::Position position = validPositions[directionToMove];
                if (position.row == -1 && position.column == -1)
                {
                    continue;
                }
                Game::GameBoard newBoard = currGameBoard.clone();
                newBoard.path.push_back(directionToMove);
                newBoard.simulateTraversingInTheSameDirection();
                string stateString = newBoard.getStateStringWithGivenKeys(currGameBoard.currentPenguPosition, newBoard.currentPenguPosition, newBoard.fishesCaughtWhileTraversing);
                if (visitedPaths.find(stateString) == visitedPaths.end())
                {
                    visitedPaths.insert(stateString);
                    frontier.push_back(newBoard);
                }
            }
        }
        return currGameBoard;
    }
}