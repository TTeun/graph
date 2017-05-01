#ifndef PARSER__H
#define PARSER__H

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <string>
#include <unordered_map>
#include "../token.h"

class Parser {
public:
  static std::queue<Token> *parse_input(std::string *input);

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
  static std::queue<Token> *to_queue(std::vector<Token> &token_list);
  static bool get_next_token(std::string *input, size_t &str_position,MODE &mode, Token &token);
  static MODE getMode(char a, MODE last_mode);
  static TOKEN_TYPE getTokenType(std::string *str);
  static bool isNumeric(char a);
  static bool isLetter(char a);
  static bool isSimpleOP(char a, MODE last_mode);
  static bool isOpenBracket(char a);
  static bool isCloseBracket(char a);
  static bool is_unary_minus(char a, MODE last_mode);
  static bool readNum(std::string *input, size_t &str_position, Parser::MODE &mode, Token &token);
  static bool readWord(std::string *input, size_t &str_position, Parser::MODE &mode, Token &token);
};

inline bool Parser::isNumeric(char a){
  return  (a >= '0') && (a <= '9');
}

inline bool Parser::isLetter(char a){
  return (a >= 'a') && (a <= 'z');
}

inline bool Parser::isSimpleOP(char a, Parser::MODE last_mode){
  return
        ( (last_mode != Parser::MODE::UNARY_MINUS)     &&
          (last_mode != Parser::MODE::UNARY_OPERATOR)  &&
          (last_mode != Parser::MODE::BINARY_OPERATOR) &&
          (last_mode != Parser::MODE::START)  )
    &&  ( (a == '+') ||
          (a == '-') || // This is a binary minus if we reach here
          (a == '*') ||
          (a == '/') ||
          (a == '^') );
}

inline bool Parser::isOpenBracket(char a){
  return (a == '(');
}

inline bool Parser::isCloseBracket(char a){
  return (a == ')');
}

inline bool Parser::is_unary_minus(char a, Parser::MODE last_mode){
  return (a == '-')                                   &&
         ( (last_mode == Parser::MODE::START)         ||
           (last_mode == Parser::MODE::CLOSE_BRACKET) ||
           (last_mode == Parser::MODE::OPEN_BRACKET) );
}

#endif
