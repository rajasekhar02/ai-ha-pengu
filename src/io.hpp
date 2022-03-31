#include <iostream>
#include <vector>
#include <string>
#include "mpi.h"
#ifndef IO_H
#define IO_H
/* All process ranks < HUGE */

namespace IO
{
    extern int IO_KEY;
    const int HUGE = 32768;
    const int NO_IO_ATTR = -1;
    const int FILE_ERROR = -2;
    std::vector<std::string> getDataFromFile(std::string filePath);
    int getDataFromFileParallel(MPI_Comm io_comm, std::string filePath, std::vector<std::string> &lines);
    std::pair<int, int> getGridSizeFromInputData(std::vector<std::string> lines);
    std::string *getGridFromInputData(std::vector<std::string> lines);
    int Cache_io_rank(
        MPI_Comm orig_comm /* in     */,
        MPI_Comm io_comm /* in/out */);
    int Copy_attr(
        MPI_Comm comm1 /* in     */,
        MPI_Comm comm2 /* in/out */,
        int KEY /* in     */);
    void Get_corresp_rank(
        MPI_Comm comm1 /* in  */,
        int rank1 /* in  */,
        MPI_Comm comm2 /* in  */,
        int *rank2_ptr /* out */);

    int Get_io_rank(
        MPI_Comm io_comm /* in  */,
        int *io_rank_ptr /* out */);

    int Cerror_test(
        MPI_Comm io_comm /* in */,
        char *routine_name /* in */,
        int error /* in */);
}
#endif