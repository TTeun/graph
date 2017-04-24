#ifndef PARSER__H
#define PARSER__H

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <string>
#include <unordered_map>
#include "../token.h"
#include "../expression/expression.h"

class Parser {
public:
  static Expression *parse_input(std::string *input);

  enum MODE{
    NUMERIC,
    LETTER,
    UNARY_MINUS,
    UNARY_OPERATOR,
    BINARY_OPERATOR,
    OPEN_BRACKET,
    CLOSE_BRACKET,
    ERROR,
    START
  };

private:
  static Expression *to_queue(std::vector<Token> &token_list);
  static bool get_next_token(std::string *input, size_t &str_position,MODE &mode, Token &token);
  static MODE getMode(char a, MODE last_mode);

};


#endif
