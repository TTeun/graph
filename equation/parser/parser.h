#ifndef PARSER__H
#define PARSER__H

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include "../token.h"

class Parser {
public:
  static std::queue<Token> *parse_input(std::string *input);

private:
  static std::queue<Token> *to_queue(std::vector<Token> &token_list);
};


#endif
