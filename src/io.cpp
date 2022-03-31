#include <fstream>
#include <cstring>
#include "utils.hpp"
#include "io.hpp"
using namespace std;
/* unused */
void *extra_arg;

/* Key identifying IO_Attribute */
int IO::IO_KEY = MPI_KEYVAL_INVALID;

vector<string> IO::getDataFromFile(string filePath)
{
    cout << filePath << endl;
    ifstream input(filePath);
    vector<string> lines;
    if (!input.is_open())
        return {""};
    while (!input.eof())
    {
        string line;
        getline(input, line);
        lines.push_back(line);
    }
    return lines;
}
pair<int, int> IO::getGridSizeFromInputData(vector<string> lines)
{
    vector<string> gridSize = utils::split(lines[0], ' ');
    return make_pair(stoi(utils::trim(gridSize[0])), stoi(utils::trim(gridSize[1])));
}
void IO::getGridSizeFromInputData(vector<string> lines, int &gridRow, int &gridCol)
{
    vector<string> gridSize = utils::split(lines[0], ' ');
    cout << lines[0] << endl;
    gridRow = stoi(utils::trim(gridSize[0]));
    gridCol = stoi(utils::trim(gridSize[1]));
}
string *IO::getGridFromInputData(vector<string> lines)
{
    vector<string> grid(lines.begin() + 1, lines.end());
    string *gridStringArr = new string[grid.size()];
    for (int i = 0; i < grid.size(); i++)
    {
        gridStringArr[i] = utils::trim(grid[i]);
    }
    return gridStringArr;
}

/********************************************************/
/* Determines whether the process with rank rank1 in
 *     comm1 is a valid rank in comm2.
 * If it is, it returns the rank in *rank2_ptr.  If it
 *     isn't it returns MPI_UNDEFINED.
 *
 */
void IO::Get_corresp_rank(
    MPI_Comm comm1 /* in  */,
    int rank1 /* in  */,
    MPI_Comm comm2 /* in  */,
    int *rank2_ptr /* out */)
{

    MPI_Group group1;
    MPI_Group group2;

    MPI_Comm_group(comm1, &group1);
    MPI_Comm_group(comm2, &group2);

    MPI_Group_translate_ranks(group1, 1,
                              &rank1, group2, rank2_ptr);

} /* Get_corresp_rank */
/********************************************************/
/* Get attribute value associated with attribute key KEY
 *     in comm1, and cache with comm2 IO_KEY
 *
 * KEY can be either IO_KEY or MPI_IO.
 *
 * Return values:
 *     1.  0:  valid attribute successfully cached.
 *     2.  NO_IO_ATTR:  Couldn't find process that could
 *         carry out I/O.  MPI_PROC_NULL is cached with
 *         comm2.
 */
int IO::Copy_attr(
    MPI_Comm comm1 /* in     */,
    MPI_Comm comm2 /* in/out */,
    int KEY /* in     */)
{

    int io_rank;
    int temp_rank;
    int *io_rank_ptr;
    int equal_comm;
    int flag;

#ifdef CDEBUG
    int my_rank;
    MPI_Comm_rank(comm1, &my_rank);
    printf("Process %d > In Copy_attr \n",
           my_rank);
#endif

    MPI_Comm_get_attr(comm1, KEY, &io_rank_ptr, &flag);

#ifdef CDEBUG
    if (flag == 0)
    {
        printf("Process %d > Attr_get failed\n", my_rank);
    }
    else if (*io_rank_ptr == MPI_ANY_SOURCE)
    {
        printf("Process %d > attr = MPI_ANY_SOURCE\n",
               my_rank);
        printf("Process %d > MPI_ANY_SOURCE = %d\n",
               my_rank, MPI_ANY_SOURCE);
    }
    else
    {
        printf("Process %d > attr = %d\n",
               my_rank, *io_rank_ptr);
    }
#endif

    if (flag == 0)
    {
        /* Attribute not cached with comm1 */
        io_rank_ptr = (int *)malloc(sizeof(int));
        *io_rank_ptr = MPI_PROC_NULL;
        MPI_Comm_set_attr(comm2, IO_KEY, io_rank_ptr);
        return NO_IO_ATTR;
    }
    else if (*io_rank_ptr == MPI_PROC_NULL)
    {
        MPI_Comm_set_attr(comm2, IO_KEY, io_rank_ptr);
        return NO_IO_ATTR;
    }
    else if (*io_rank_ptr == MPI_ANY_SOURCE)
    {
        /* Any process can carry out I/O.  Use */
        /* process 0                           */
#ifdef CDEBUG
        printf("Process %d > Returning from Copy on MPI_ANY_SOURCE\n",
               my_rank);
#endif
        io_rank_ptr = (int *)malloc(sizeof(int));
        *io_rank_ptr = 0;
        MPI_Comm_set_attr(comm2, IO_KEY, io_rank_ptr);
#ifdef CDEBUG
        {
            int *temp_ptr;
            MPI_Attr_get(comm2, IO_KEY, &temp_ptr, &flag);
            if (flag == 0)
                printf("Process %d > In Copy, no value cached!\n", my_rank);
            else
                printf("Process %d > In Copy, cached io_rank = %d\n",
                       my_rank, *temp_ptr);
        }
#endif
        return 0;
    }

    /* Value in *io_rank_ptr is a valid process  */
    /* rank in comm1.  Action depends on whether */
    /* comm1 == comm2.                           */
    MPI_Comm_compare(comm1, comm2, &equal_comm);

    if (equal_comm == MPI_IDENT)
    {
        /* comm1 == comm2.  Valid value already */
        /* cached.  Do nothing.                 */
        return 0;
    }
    else
    {
        /* Check whether rank returned is valid */
        /* process rank in comm2                */
        Get_corresp_rank(comm1, *io_rank_ptr,
                         comm2, &temp_rank);

        /* Different ranks may have been returned */
        /* on different processes.  Get min       */
        if (temp_rank == MPI_UNDEFINED)
            temp_rank = HUGE;
        MPI_Allreduce(&temp_rank, &io_rank, 1, MPI_INT,
                      MPI_MIN, comm2);

        io_rank_ptr = (int *)malloc(sizeof(int));
        if (io_rank < HUGE)
        {
            *io_rank_ptr = io_rank;
            MPI_Comm_set_attr(comm2, IO_KEY, io_rank_ptr);
            return 0;
        }
        else
        {
            /* No process got a valid rank in comm2 */
            /* from Get_corresp_rank                */
            *io_rank_ptr = MPI_PROC_NULL;
            MPI_Comm_set_attr(comm2, IO_KEY, io_rank_ptr);
            return NO_IO_ATTR;
        }
    }
} /* Copy_attr  */
/********************************************************/
/* Check whether IO_KEY is valid.  If it is, attempt to
 *     access it.  If it isn't attempt to define it from
 *     MPI_COMM_WORLD.
 *
 * Return values:
 *     1.  0: Valid rank returned.
 *     2.  NO_IO_ATTR:  Unable to find rank.
 */
int IO::Get_io_rank(
    MPI_Comm io_comm /* in  */,
    int *io_rank_ptr /* out */)
{

    int *temp_ptr;
    int flag;

    if (IO_KEY == MPI_KEYVAL_INVALID)
    {
        MPI_Comm_create_keyval(MPI_COMM_DUP_FN,
                               MPI_COMM_NULL_DELETE_FN, &IO::IO_KEY, extra_arg);
    }
    else
    {
        MPI_Comm_get_attr(io_comm, IO_KEY, &temp_ptr, &flag);
        if ((flag != 0) && (*temp_ptr != MPI_PROC_NULL))
        {
            *io_rank_ptr = *temp_ptr;
            return 0;
        }
    }

    if (Copy_attr(MPI_COMM_WORLD, io_comm, MPI_IO) == NO_IO_ATTR)
    {
        return NO_IO_ATTR;
    }
    else
    {
        MPI_Comm_get_attr(io_comm, IO_KEY, &temp_ptr, &flag);
        if ((flag != 0) && (*temp_ptr != MPI_PROC_NULL))
        {
            *io_rank_ptr = *temp_ptr;
            return 0;
        }
    }
    return -3;
} /* Get_io_rank */
/********************************************************/
/* Attempt to identify a process in io_comm that can be
 *     used for I/O.
 *
 * First see whether program defined io rank has been
 *     cached with either communicator.  If this fails
 *     try MPI defined io rank.
 *
 * Return values:
 *     1.  0: rank of I/O process cached with io_comm.
 *     2.  NO_IO_ATTR: couldn't find processor that could
 *         carry out I/O.  MPI_PROC_NULL cached with
 *         io_comm.
 *
 * Notes:
 *     1.  This is a collective operation, since function
 *         Copy_attr may use collective comm.
 *     2.  Only possible values cached are a valid process
 *         rank in comm2 or MPI_PROC_NULL.  (MPI_ANY_SOURCE
 *         won't be cached.)
 */
int IO::Cache_io_rank(
    MPI_Comm orig_comm /* in     */,
    MPI_Comm io_comm /* in/out */)
{

    int retval; /* 0 or NO_IO_ATTR */

#ifdef CDEBUG
    int my_rank;
    MPI_Comm_rank(orig_comm, &my_rank);

    printf("Process %d > In Cache_io_rank\n", my_rank);
#endif

    /* Check whether IO_KEY is defined.  If not, define */
    if (IO_KEY == MPI_KEYVAL_INVALID)
    {
        MPI_Comm_create_keyval(MPI_COMM_DUP_FN,
                               MPI_COMM_NULL_DELETE_FN, &IO_KEY, extra_arg);
    }
    else if ((retval = Copy_attr(io_comm, io_comm,
                                 IO_KEY)) != NO_IO_ATTR)
    {
        /* Value cached */
        return retval;
    }
    else if ((retval = Copy_attr(orig_comm, io_comm,
                                 IO_KEY)) != NO_IO_ATTR)
    {
        /* Value cached */
        return retval;
    }

#ifdef CDEBUG
    printf("Process %d > In Cache_io_rank, give up on IO_KEY\n",
           my_rank);
#endif
    /* Now see if we can find a value cached for MPI_IO */
    if ((retval = Copy_attr(orig_comm, io_comm,
                            MPI_IO)) != NO_IO_ATTR)
    {
        /* Value cached */
#ifdef CDEBUG
        printf("Process %d > Copied attribute from orig to io\n",
               my_rank);
#endif
        return retval;
    }
    else if ((retval = Copy_attr(io_comm, io_comm,
                                 MPI_IO)) != NO_IO_ATTR)
    {
        /* Value cached */
#ifdef CDEBUG
        printf("Process %d > Copied attribute from io to io\n",
               my_rank);
#endif
        return retval;
    }

#ifdef CDEBUG
    printf("Process %d > In Cache_io_rank, returning at end\n",
           my_rank);
#endif
    /* Couldn't find process that could carry out I/O */
    /* Copy_attr has cached MPI_PROC_NULL             */
    return NO_IO_ATTR;

} /* Cache_io_rank */

int IO::getDataFromFileParallel(MPI_Comm io_comm, string filePath, vector<string> &lines)
{
    int my_io_rank;
    int root;
    if (Get_io_rank(io_comm, &root) == IO::NO_IO_ATTR)
        return IO::NO_IO_ATTR;
    MPI_Comm_rank(io_comm, &my_io_rank);
    vector<string> inputLines;
    int linesCount = 0;
    if (my_io_rank == root)
    {
        ifstream input(filePath);
        if (!input.is_open())
            return IO::FILE_ERROR;
        while (!input.eof())
        {
            string line;
            getline(input, line);
            inputLines.push_back(line);
        }
        cout << inputLines.size() << endl;
        linesCount = (int)inputLines.size();
    }
    // MPI_Barrier(io_comm);
    MPI_Bcast(&linesCount, 1, MPI_INT, root, io_comm);
    // broadcasting each line size
    int lin_siz[linesCount];
    if (my_io_rank == root)
    {
        for (int i = 0; i < linesCount; i++)
        {
            char *line = &inputLines[i][0];
            lin_siz[i] = strlen(line) + 1;
        }
    }
    MPI_Bcast(&lin_siz, linesCount, MPI_INT, root, io_comm);
    for (int i = 0; i < linesCount; i++)
    {
        char *line = (char *)malloc(lin_siz[i]);
        if (my_io_rank == root)
        {
            line = &inputLines[i][0];
        }
        MPI_Bcast(line, lin_siz[i], MPI_CHAR, root, io_comm);
        // cout << line << " after " << my_io_rank << endl;
        lines.push_back(string(line));
    }
    // cout << lines.size() << "my_io_rank" << my_io_rank << endl;
    return 1;
}
