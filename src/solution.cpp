#include <stdio.h>
#include "mpi.h"
#include "solution.hpp"

static Game::GameBoard *best_solution;
static Game::GameBoard *temp_solution;
static Game::GameBoard *solution1;
static Game::GameBoard *solution2;
static Game::GameBoard solution_size;

namespace solution
{
    /*********************************************************************/
    /* Check queue for new solutions.  Return best solution found */
    Game::GameBoard Best_solution(
        MPI_Comm comm /* in */)
    {
        MPI_Status probe_status;
        MPI_Status recv_status;
        bool done = false;
        int message_pending;
        int *temp;

        while (!done)
        {
            MPI_Iprobe(MPI_ANY_SOURCE, solution::SOLUTION_TAG, comm,
                       &message_pending, &probe_status);
            if (message_pending)
            {
                MPI_Recv(temp_solution, 100, MPI_INT,
                         probe_status.MPI_SOURCE, SOLUTION_TAG, comm,
                         &recv_status);
                // if (*temp_solution < *best_solution)
                // {
                //     temp = temp_solution;
                //     temp_solution = best_solution;
                //     best_solution = temp;
                // }
            }
            else
            {
                done = true;
            }
        } /* while */

        return Local_best_solution();
    } /* Best_solution */
}
