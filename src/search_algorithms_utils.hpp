#ifndef SEARCH_ALGORITHMS_UTILS_H
#define SEARCH_ALGORITHMS_UTILS_H
#include "GameBoard.hpp"
#include "mpi.h"
namespace search_algorithm_utils
{
    Game::GameBoard *best_solution;
    bool check_found_solution(Game::GameBoard gmBoard);
    Game::GameBoard getBestSolutionFoundByOtherProcesses(MPI_Comm comm);
    void Par_dfs(deque<string> &frontier, MPI_Comm comm);
    Game::GameBoard getBestSolutionFoundByOtherProcesses(MPI_Comm comm);
    void Local_solution_update(Game::GameBoard *bestSolutionGameBoard);
    void Bcast_Solution(MPI_Comm comm);
    void Free_soln(void);
    void Update_solution(MPI_Comm comm);
}
#endif