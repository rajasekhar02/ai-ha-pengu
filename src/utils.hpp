//
// Created by rp6kp on 3/14/2022.
//

#include<string>
#include<vector>


#ifndef GAME_BOARD_UTILS_H
#define GAME_BOARD_UTILS_H
namespace utils {
    std::vector<std::string> split(std::string s, char c = ' ');
    int charToInt(char ch);
}
#endif //GAME_BOARD_UTILS_H