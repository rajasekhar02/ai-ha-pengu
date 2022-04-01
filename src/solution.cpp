#include <stdio.h>
#include <string>
#include "mpi.h"
#include "solution.hpp"
#include "GameBoard.hpp"

static Game::GameBoard best_solution;
static Game::GameBoard temp_solution;
static Game::GameBoard *solution1;
static Game::GameBoard *solution2;
static Game::GameBoard solution_size;
using namespace std;
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
        Game::GameBoard temp;

        while (!done)
        {
            MPI_Iprobe(MPI_ANY_SOURCE, solution::SOLUTION_TAG, comm,
                       &message_pending, &probe_status);
            if (message_pending)
            {
                int line_size;
                MPI_Get_count(&probe_status, MPI_CHAR, &line_size);
                char *gameBoardString = (char *)malloc(line_size);
                MPI_Recv(gameBoardString, line_size, MPI_CHAR,
                         probe_status.MPI_SOURCE, SOLUTION_TAG, comm,
                         &recv_status);
                temp_solution = Game::GameBoard::castStringToGameBoard(string(gameBoardString));
                if (temp_solution.path.size() < best_solution.path.size())
                {
                    temp = temp_solution;
                    temp_solution = best_solution;
                    best_solution = temp;
                }
            }
            else
            {
                done = true;
            }
        } /* while */

        return Local_best_solution();
    } /* Best_solution */

    /*********************************************************************/
    Game::GameBoard Local_best_solution(void)
    {
        return best_solution;
    } /* Local_best_solution */

    /********************************************************************/
    /* Assumes plenty of system buffering.  So beware! */
    void Bcast_solution(MPI_Comm comm)
    {
        int my_rank;
        int p;
        int q;

        MPI_Comm_rank(comm, &my_rank);
        MPI_Comm_size(comm, &p);
        for (q = 0; q < my_rank; q++)
        {
            string best_solution_str = best_solution.castGameBoardToString();
            char *best_solution_char = &best_solution_str[0];
            MPI_Send(best_solution_char, strlen(best_solution_char) + 1, MPI_CHAR, q,
                     SOLUTION_TAG, comm);
        }

        for (q = my_rank + 1; q < p; q++)
        {
            string best_solution_str = best_solution.castGameBoardToString();
            char *best_solution_char = &best_solution_str[0];
            MPI_Send(best_solution_char, strlen(best_solution_char) + 1, MPI_CHAR, q,
                     SOLUTION_TAG, comm);
        }

    } /* Bcast_solution */

    /*********************************************************************/
    /* Find owner of global best solution and broadcast */
    void Update_solution(MPI_Comm comm)
    {
        GameBoardSolution local_data;
        GameBoardSolution global_data;

        local_data.solnPathLength = Local_best_solution().path.size();
        MPI_Comm_rank(comm, &(local_data.rank));

        MPI_Allreduce(&local_data, &global_data, 1, update_mpi_t,
                      MPI_MINLOC, comm);
        string best_solution_str = best_solution.castGameBoardToString();
        char *best_solution_char = &best_solution_str[0];
        MPI_Bcast(best_solution_char, strlen(best_solution_char) + 1, MPI_INT,
                  global_data.rank, comm);
    } /* Update_solution */

    /*********************************************************************/
    void Free_soln(void)
    {
        free(&best_solution);
        free(&temp_solution);
    } /* Free_soln */
    /*********************************************************************/
    /* Return 0 if OK*/
    int Initialize_soln(
        int max_depth /* in */)
    {
        int i;
        // Game::GameBoard
        //     best_solution = solution1;
        // temp_solution = solution2;
        // best_solution[0] = INFINITY;

        // best_solution[1] = 0; /* root is on best path */
        // for (i = 1; i < solution_size; i++)
        //     best_solution[i] = -1;

        return 0;
    } /* Initialize_soln */
}
