/* service_requests.h
 *
 * Definitions and declarations used by the service_requests functions
 */
#ifndef SERVICE_REQUESTS_H
#define SERVICE_REQUESTS_H

#include "node_stack.hpp"
#include "mpi.h"

namespace service_requests
{
    /* Called in Service_requests */
    int Allocate_type_arrays(int cutoff_depth, int max_children);
    void Free_type_arrays(void);
    void Service_requests(node_stack::StringStack local_stack, MPI_Comm comm);
    bool Nodes_available(node_stack::StringStack local_stack);
    void Split(node_stack::StringStack local_stack, node_stack::StringStack *send_stack_ptr);
    void Send_work(int destination, node_stack::StringStack send_stack, MPI_Comm comm);
    void Compress(node_stack::StringStack stack);
    void Send_reject(int destination, MPI_Comm comm);
    void Send_all_rejects(MPI_Comm comm);
}
#endif
