#include "GameBoard.hpp"
#include <deque>
#ifndef PAR_TREE_SEARCH_H
#define PAR_TREE_SEARCH_H

#include "mpi.h"

namespace parallel_search_algorithms
{
    Game::GameBoard parallelTreeSearch(Game::GameBoard initGameBoard, MPI_Comm common_type);
    void Generate(Game::GameBoard root, std::deque<Game::GameBoard> &frontier, int p, MPI_Comm comm);
    void Scatter(std::deque<Game::GameBoard> &frontier, MPI_Comm comm);
    void PTS_expand(Game::GameBoard currentNode, std::deque<Game::GameBoard> &frontier);
}

#endif
