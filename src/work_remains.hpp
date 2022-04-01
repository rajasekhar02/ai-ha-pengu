/* work_remains.h
 *
 * Definitions and declarations used by the work_remains functions
 */
#ifndef WORK_REMAINS_H
#define WORK_REMAINS_H

#include "mpi.h"
#include "node_stack.hpp"

namespace work_remains
{
    /* Called in Work_remains */
    int Work_remains(node_stack::StringStack local_stack, MPI_Comm comm);
    void Send_energy(MPI_Comm comm);
    int Search_complete(MPI_Comm comm);
    bool New_request(MPI_Comm comm);
    void Send_request(int work_request_process, MPI_Comm comm);
    void handle_IReceive_for_Send_request(MPI_Comm comm, int work_request_process);
    bool Reply_received(int work_request_process, int *work_available,
                        node_stack::StringStack local_stack, MPI_Comm comm);
    void Cancel_request(void);
}

#endif
