/* service_requests.c -- functions for handling requests for work
 *    from other processes.
 *
 * See Chap 14, p. 332, in PPMPI.
 */
#include <stdlib.h>
#include "service_requests.hpp"
#include "queue.hpp"
#include "terminate.hpp"
#include "node_stack.hpp"
#ifdef STATS
#include "stats.h"
#endif

// value will assigned by the Allocate_type_arrays function
extern int cutoff_depth;
extern MPI_Comm io_comm;

MPI_Datatype send_stack_mpi_t;

static int allocated_size;
static int *block_lengths = (int *)NULL;
static int *displacements = (int *)NULL;
static node_stack::StringStack *node_list;
static int node_count;

namespace service_requests
{
    /*********************************************************************/
    void Free_type_arrays(void)
    {
        free(block_lengths);
        free(displacements);
        free(node_list);
    } /* Free_type_arrays */

    /*********************************************************************/
    /* Use MPI_Iprobe (via Work_requests_pending) to see whether other
     *     processes have requested work.  If so, check local stack to
     *     see whether there are any nodes above cutoff_depth.  If so,
     *     send half of them to the requesting process.  If not, send
     *     a "reject" message.  The splitting of the stack is done using
     *     MPI_Type_indexed.  After the data has been sent.  The stack
     *     is "compressed".
     */
    void Service_requests(
        node_stack::StringStack local_stack /* in/out */,
        MPI_Comm comm /* in     */)
    {
        char *send_string;
        int destination;
        while (request_queue::Work_requests_pending(comm))
        {
            destination = request_queue::Get_dest(comm);

            if (Nodes_available(local_stack))
            {
                Split(local_stack, &send_string);
                Send_work(destination, send_string, comm);
            }
            else
            {
                Send_reject(destination, comm);
            }
        }

    } /* Service_requests */

    /*********************************************************************/
    /* Count until at least two nodes above cutoff depth are found */
    /*     Should probably be combined with Split                  */
    bool Nodes_available(
        node_stack::StringStack local_stack /* in */)
    {
        int count = local_stack.size() - cutoff_depth;
        if (count >= 2)
            return true;
        else
            return false;
    } /* Nodes_available */

    /*********************************************************************/
    /* Split builds a derived datatype that picks out the nodes to be */
    /*     sent */
    void Split(
        node_stack::StringStack local_stack /* in  */,
        char *send_stack_ptr /* out */)
    {
        int index = 0;
        node_stack::StringStack node;
        int odd = 0;

        node = Stack_list(local_stack);
        node_count = 0;
        while ((node != NODE_NULL) && (node <= Top(local_stack)))
        {
            if (Depth(node) <= cutoff_depth)
                if (odd)
                {
                    block_lengths[node_count] = Size(node);
                    displacements[node_count] = index;
                    node_list[node_count] = node;
                    node_count++;
                    odd = 0; /* skip the next one above cutoff */
                }
                else
                {
                    odd = 1; /* take the next one above cutoff */
                }
            index = index + Size(node);
            node = Next(local_stack, node);
        }

        MPI_Type_indexed(node_count, block_lengths, displacements, MPI_INT,
                         &send_stack_mpi_t);
        MPI_Type_commit(&send_stack_mpi_t);

        Get_local_stack(send_stack_ptr);

    } /* Split */

    /*********************************************************************/
    void Send_work(
        int destination /* in */,
        STACK_T send_stack /* in */,
        MPI_Comm comm /* in */)
    {

        MPI_Send(Stack_list(send_stack), 1, send_stack_mpi_t, destination,
                 WORK_TAG, comm);
        MPI_Type_free(&send_stack_mpi_t);
        Send_half_energy(destination, comm);
        Compress(send_stack);
    } /* Send_work */

    /*********************************************************************/
    void Compress(
        STACK_T stack /* in/out */)
    {
        NODE_T compress_point; /* In general, not a node, just a point */
                               /*     in the stack                     */
        NODE_T delete_node;
        NODE_T save_node;
        int new_in_use;
        int i;
        compress_point = node_list[0];
        new_in_use = displacements[0];

        for (i = 0; i < node_count; i++)
        {
            delete_node = node_list[i];
            if (Next(stack, delete_node) <= Top(stack))
            {
                save_node = Next(stack, delete_node);
                Copy_node(save_node, compress_point);
                new_in_use = new_in_use + Size(compress_point);
                compress_point = Next(stack, compress_point);
            }
        }

        Top(stack) = Pred(stack, compress_point);
        In_use(stack) = new_in_use;
    } /* Compress */

    /*********************************************************************/
    void Send_reject(
        int destination /* in */,
        MPI_Comm comm /* in */)
    {
        int x = -1;
        MPI_Send(&x, 1, MPI_INT, destination, WORK_TAG, comm);
    } /* Send_reject */

    /*********************************************************************/
    void Send_all_rejects(
        MPI_Comm comm)
    {
        int destination;

        while (request_queue::Work_requests_pending(comm))
        {
            destination = request_queue::Get_dest(comm);
            Send_reject(destination, comm);
        }
    } /* Send_all_rejects */
}