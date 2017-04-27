#ifndef TOKEN__H
#define TOKEN__H

#include <string>

enum class TOKEN_TYPE{
  NUM,
  VAR,
  UNARY_OP,
  BINARY_OP,
  BRA,
  NONE
};

struct Token {
  Token();
  Token(TOKEN_TYPE _type, std::string &&str);
  Token(TOKEN_TYPE _type, std::string const &str);
  TOKEN_TYPE type = TOKEN_TYPE::NONE;
  std::string value{};

  void printToken();
  void printVal();
};

#endif
