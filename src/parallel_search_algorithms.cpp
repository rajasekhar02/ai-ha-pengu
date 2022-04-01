#include "GameBoard.hpp"
#include "parallel_search_algorithms.hpp"
#include <deque>
#include <vector>
#include <iostream>
#include <set>
using namespace std;
namespace parallel_search_algorithms
{
    int p;
    int my_rank;

    Game::GameBoard parallelTreeSearch(
        Game::GameBoard initGameBoard, /*in*/
        MPI_Comm comm /*in*/)
    {
        deque<Game::GameBoard> frontier;
        const int totalFishCount = Game::GameBoard::getTotalFishCount();
        MPI_Comm_size(comm, &p);
        MPI_Comm_rank(comm, &my_rank);
        if (my_rank == 0)
        {
            Generate(initGameBoard, frontier, p, comm);
        }
    }

    void Generate(Game::GameBoard initGameBoard, deque<Game::GameBoard> &frontier, int size, MPI_Comm comm)
    {
        Game::GameBoard node;
        const int totalFishCount = Game::GameBoard::getTotalFishCount();
        while (!frontier.empty())
        {
            node = frontier.back();
            frontier.pop_back();
            if (node.fishesCaughtWhileTraversing.size() == totalFishCount)
            {
            }
        }
    }

}