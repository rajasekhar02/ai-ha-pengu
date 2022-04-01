#include <iostream>
#include "utils.hpp"
#include "GameBoard.hpp"
#include "io.hpp"
#include "search_algorithms.hpp"
#include "terminate.hpp"
#include "node_stack.hpp"
#include "service_requests.hpp"

using namespace std;
using namespace utils;

/* Global variables */
int max_work;
int cutoff_depth;
int max_children;

MPI_Comm io_comm;
int p;
int my_rank;

// void Get_root(NODE_T *root_ptr);
/*----------------------------------*/
void serialDFS(string filePath)
{
    vector<string> lines = IO::getDataFromFile(filePath);

    pair<int, int> gridSize = IO::getGridSizeFromInputData(lines);

    string *grid = IO::getGridFromInputData(lines);

    Game::GameBoard::initGrid(grid, gridSize.first, gridSize.second);

    Game::GameBoard::initSymbolPositions();

    Game::Position currentPenguPosition = Game::GameBoard::getPenguPositionFromInput();

    Game::GameBoard::clearPenguPositionOnGrid();

    Game::GameBoard gmBoard(currentPenguPosition, Game::Status::INITIAL);

    gmBoard = search_algorithms::depthFirstSearch(gmBoard);

    cout << gmBoard;

    Game::GameBoard gmBoardChecker(currentPenguPosition, Game::Status::INITIAL);

    for (int i = 0; i < gmBoard.path.size(); i++)
    {
        gmBoardChecker.path.push_back(gmBoard.path[i]);
        gmBoardChecker.simulateTraversingInTheSameDirection();
    }

    cout << gmBoardChecker;

    cout << ((gmBoard.fishesCaughtWhileTraversing.size() == gmBoardChecker.fishesCaughtWhileTraversing.size()) ? "Accepted" : "Wrong Answer") << endl;
}
void parallelDFS(int argc, char **argv)
{
    int error;
    // NODE_T root;
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &p);

    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    MPI_Comm_dup(MPI_COMM_WORLD, &io_comm);

    IO::Cache_io_rank(MPI_COMM_WORLD, io_comm);

    string filePath = string(argv[1]);

    node_stack::max_depth = stoi(string(argv[2]));

    service_requests::cutoff_depth = stoi(string(argv[3]));

    max_work = stoi(string(argv[4]));

    max_children = stoi(string(argv[5]));

    vector<string> lines;

    IO::getDataFromFileParallel(io_comm, filePath, lines);

    cout << my_rank << endl;

    cout << "success" << endl;

    int gridRow;

    int gridCol;

    IO::getGridSizeFromInputData(lines, gridRow, gridCol);

    string *grid = IO::getGridFromInputData(lines);

    Game::GameBoard::initGrid(grid, gridRow, gridCol);

    Game::GameBoard::initSymbolPositions();

    Game::Position currentPenguPosition = Game::GameBoard::getPenguPositionFromInput();

    Game::GameBoard::clearPenguPositionOnGrid();

    Game::GameBoard gmBoard(currentPenguPosition, Game::Status::INITIAL);

    cout << gmBoard << endl;

    terminate::Setup_term_detect();

    // error = Initialize_soln(max_depth);
    // Cerror_test(io_comm, "Initialize_soln", error);

    cout << "completed" << endl;

    MPI_Finalize();
}
int main(int argc, char **argv)
{
    // serialDFS(string(argv[2]));
    parallelDFS(argc, argv);
    return 0;
}
