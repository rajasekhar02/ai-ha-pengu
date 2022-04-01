/* solution.h
 *
 * Declarations and definitions for manipulating the solutions
 */
#ifndef SOLUTION_H
#define SOLUTION_H
#include "mpi.h"
#include "GameBoard.hpp"
#define update_mpi_t MPI_2INT

namespace solution
{
    struct GameBoardSolution
    {
        int solnPathLength;
        int rank;
    };
    const int SOLUTION_TAG = 100;
    Game::GameBoard Best_solution(MPI_Comm comm);
    Game::GameBoard Local_best_solution(void);
    void Local_solution_update(Game::GameBoard cost, Game::GameBoard node);
    void Bcast_solution(MPI_Comm comm);
    int Initialize_soln(int max_depth);
    void Update_solution(MPI_Comm comm);
    void Print_local_solution(MPI_Comm comm);
    void Print_solution(MPI_Comm io_comm);
    void Free_soln(void);
}
#endif
