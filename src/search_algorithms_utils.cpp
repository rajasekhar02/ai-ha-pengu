#include "search_algorithms_utils.hpp"
#include "mpi.h"
#include <iostream>
#include <string>
namespace search_algorithm_utils
{
    int SOLUTION_TAG = 5000;
    Game::GameBoard best_solution_obj;
    Game::GameBoard *best_solution = &best_solution_obj;
    bool check_found_solution(Game::GameBoard gmBoard)
    {
        const int totalFishCount = Game::GameBoard::getTotalFishCount();
        if (gmBoard.fishesCaughtWhileTraversing.size() >= totalFishCount)
        {
            return true;
        }
        return false;
    }
    void Par_dfs(deque<string> &frontier, MPI_Comm comm)
    {
        int count;
        string node;
        int temp_solution;
        count = 0;
        while (frontier.empty() && (count < max_work))
        {
            node = frontier.back();
            Game::GameBoard currGameBoard = Game::GameBoard::castStringToGameBoard(node);
            frontier.pop_back();
            if (check_found_solution(currGameBoard))
            {
                temp_solution = node.path.size();
                int best_solution_size = getBestSolutionFoundByOtherProcesses(comm).path.size();
                if (temp_solution < best_solution)
                {
                    Local_solution_update(node);
                    Bcast_Solution(comm);
                }
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
    }
    Game::GameBoard getBestSolutionFoundByOtherProcesses(MPI_Comm comm)
    {
        MPI_Status probe_status;
        MPI_Status recv_status;
        bool done = false;
        int message_pending;
        Game::GameBoard *temp;
        while (!done)
        {
            MPI_Iprobe(MPI_ANY_SOURCE, SOLUTION_TAG, comm, &message_pending, &probe_status);
            if (message_pending)
            {
                int line_size;
                MPI_Get_count(&probe_status, MPI_CHAR, &line_size);
                std::cout << line_size << std::endl;
                char *gameBoardString = (char *)malloc(line_size);
                MPI_Recv(gameBoardString, line_size, MPI_CHAR, probe_status.MPI_SOURCE, SOLUTION_TAG, MPI_COMM_WORLD, &recv_status);
                std::string gm1 = std::string(gameBoardString);
                Game::GameBoard *temp_solution;
                Game::GameBoard temp_solution_obj = Game::GameBoard::castStringToGameBoard(gm1);
                temp_solution = &temp_solution_obj;
                if (temp_solution->path.size() < best_solution->path.size())
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
        }
        return *best_solution;
    }
    void Local_solution_update(Game::GameBoard *bestSolutionGameBoard)
    {
        best_solution = bestSolutionGameBoard;
    }
    void Bcast_Solution(MPI_Comm comm)
    {
        int my_rank;
        int p;
        int q;
        MPI_Comm_rank(comm, &my_rank);
        MPI_Comm_size(comm, &p);
        std::string gm1 = best_solution->castGameBoardToString();
        char *gameBoardString = &gm1[0];
        int line_size = strlen(gameBoardString) + 1;
        for (q = 0; q < my_rank; q++)
        {
            MPI_Send(gameBoardString, line_size, MPI_CHAR, q,
                     SOLUTION_TAG, comm);
        }
        for (q = my_rank + 1; q < p; q++)
        {
            MPI_Send(gameBoardString, line_size, MPI_CHAR, q,
                     SOLUTION_TAG, comm);
        }
    }
    /*********************************************************************/
    void Free_soln(void)
    {
        free(best_solution);
    } /* Free_soln */
    /*********************************************************************/
    /* Find owner of global best solution and broadcast */
    void Update_solution(MPI_Comm comm)
    {
        struct UPDATE_STRUCT_T
        {
            int rank;
            int soln;
        };
        UPDATE_STRUCT_T local_data;
        UPDATE_STRUCT_T global_data;

        local_data.soln = best_solution->path.size();
        MPI_Comm_rank(comm, &(local_data.rank));
        MPI_Allreduce(&local_data, &global_data, 1, MPI_2INT,
                      MPI_MINLOC, comm);
        std::string gm1 = best_solution->castGameBoardToString();
        char *gameBoardString = &gm1[0];
        int line_size = strlen(gameBoardString) + 1;
        MPI_Bcast(gameBoardString, line_size, MPI_CHAR,
                  global_data.rank, comm);
    } /* Update_solution */
}