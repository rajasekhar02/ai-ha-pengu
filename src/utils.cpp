//
// Created by rp6kp on 3/14/2022.
//

#include<string>
#include<vector>
#include"utils.hpp"

using namespace std;

vector<string> utils::split(string s, char c) {
    vector<string> res;
    string tempStr = "";
    for (int index = 0; index < s.length(); index++) {
        if (s[index] != c) {
            tempStr += s[index];
        } else {
            res.push_back(tempStr);
            tempStr = "";
        }
    }
    res.push_back(tempStr);
    return res;
}

int utils::charToInt(char ch){
    return ch - '0';
}