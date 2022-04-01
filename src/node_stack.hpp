#ifndef NODE_STACK_H
#define NODE_STACK_H
#include <deque>
#include <string>
namespace node_stack
{
    extern int max_depth;
    typedef std::deque<std::string> StringStack;
    void Get_local_stack(StringStack *local_stack_ptr);
    void parseNodesStringAndToStack(std::string arrGameBoardString);
}
#endif