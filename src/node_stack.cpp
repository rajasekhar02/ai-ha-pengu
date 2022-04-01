#include "node_stack.hpp"
#include "utils.hpp"
#include <vector>
#include <string>
namespace node_stack
{
    static StringStack local_stack;
    void Get_local_stack(StringStack *local_stack_ptr)
    {
        local_stack_ptr = &local_stack;
    }
    void parseNodesStringAndToStack(std::string arrGameBoardString)
    {
        std::vector<std::string> gameBoards = utils::split(arrGameBoardString, '<');
        for (const std::string gameBoard : gameBoards)
        {
            local_stack.push_back(gameBoard);
        }
    }
}