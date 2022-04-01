/* work_remains.c -- controls exit from main loop in parallel tree
 *     search program.  Checks whether local stack is empty.  If not,
 *     returns true.  If it is, executes the following algorithm.
 *
 *         Send "energy" to process 0 (see terminate.c, below).
 *         while(true) {
 *             Send reject messages to all processes requesting work.
 *             if (the search is complete) {
 *                 Cancel outstanding work requests.
 *                 return false.
 *             } else if (there's no outstanding request) {
 *                 Generate a process rank to which to send a request.
 *                 Send a request for work.
 *             } else if (a reply has been received) {
 *                 if (work has been received) return true.
 *             }
 *         }
 *
 * See Chap 14, pp. 332 & ff, in PPMPI.
 */
#include <stdlib.h>
#include "work_remains.hpp"
#include "node_stack.hpp"
#include "terminate.hpp"
#include "service_requests.hpp"
#include "queue.hpp"
#include <string>
extern int my_rank;
extern int p;

namespace work_remains
{
    MPI_Request posted_recv;
    /*********************************************************************/
    int Work_remains(
        node_stack::StringStack &local_stack /* in/out */,
        MPI_Comm comm /* in     */)
    {

        int work_available;
        int work_request_process;
        int request_sent;
        if (!local_stack.empty())
        {
            return true;
        }
        else
        {
            terminate::Return_energy(comm);
            request_sent = false;
            while (true)
            {
                /*
                sends reject messages to all processes that have
                requested work as local stack will empty if entered
                into this else condition
                */
                service_requests::Send_all_rejects(comm);
                if (terminate::Search_complete(comm))
                {

                    if (request_sent)
                        Cancel_request();
                    return false;
                }
                else if (!request_sent)
                {
                    work_request_process = New_request(comm);
                    Send_request(work_request_process, comm);
                    request_sent = true;
                }
                else if (Reply_received(work_request_process,

                                        &work_available, local_stack, comm))
                {
                    if (work_available)
                    {
                        return true;
                    }
                    else
                    {
                        request_sent = false;
                    }
                }

            } /* while (true) */
        }
    } /* Work_remains */

    /*********************************************************************/
    /* Generates a process rank to which a new request should be sent --
     *     simply rand() % p
     */
    bool New_request(
        MPI_Comm comm /* in */)
    {
        static int seeded = false;
        int rank;

        if (!seeded)
        {
            srand(my_rank);
            seeded = true;
        }

        while ((rank = rand() % p) == my_rank)
            ;

        return rank;
    } /* New_request */

    /*********************************************************************/
    void Send_request(
        int work_request_process /* in  */,
        MPI_Comm comm /* out */)
    {
        int x = 0;
        MPI_Send(&x, 1, MPI_INT, work_request_process, REQUEST_TAG,
                 comm);
        /* Post nonblocking receive which will be used by the reply_received method using posted_recv*/
        handle_IReceive_for_Send_request(comm, work_request_process);
    } /* Send_request */

    void handle_IReceive_for_Send_request(MPI_Comm comm, int work_request_process)
    {
        int message_pending;
        MPI_Status status;
        MPI_Iprobe(MPI_ANY_SOURCE, WORK_TAG, comm, &message_pending, &status);
        if (message_pending)
        {
            int linesCount = 0;
            MPI_Get_count(&status, MPI_CHAR, &linesCount);
            char *arrGameBoardString = (char *)malloc(linesCount);
            MPI_Irecv(arrGameBoardString, linesCount, MPI_CHAR, work_request_process, WORK_TAG, comm,
                      &posted_recv);
            node_stack::parseNodesStringAndToStack(std::string(arrGameBoardString));
        }
    }

    /*********************************************************************/
    bool Reply_received(
        int work_request_process /* in  */,
        int *work_available /* out */,
        node_stack::StringStack local_stack /* in */,
        MPI_Comm comm /* in  */)
    {
        int reply_received;
        MPI_Status status;
        int count;

        MPI_Test(&posted_recv, &reply_received, &status);

        if (reply_received)
        {
            if (local_stack.empty())
            {
                *work_available = false;
            }
            else
            {
                *work_available = true;
            }
            return true;
        }
        else
        {
            return false;
        }
    } /* Reply_received */

    /*********************************************************************/
    /* Tree search has completed, but there is still an outstanding work */
    /*    request.  Try to cancel it.                                    */
    void Cancel_request(void)
    {
        MPI_Status status;

        /*  Cancel not implemented in mpich prior to 1.0.13 */
        MPI_Cancel(&posted_recv);
        MPI_Wait(&posted_recv, &status);

    } /* Cancel_request */
}