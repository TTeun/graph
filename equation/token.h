#ifndef TOKEN__H
#define TOKEN__H

#include <string>
#include <iostream>

enum class TOKEN_TYPE{
  NUM,
  VAR,
  UNARY_OP,
  BINARY_OP,
  BRA,
  NONE
};

struct Token{
  TOKEN_TYPE type = TOKEN_TYPE::NONE;
  std::string value{};

  void printToken(){
    std::cout << "(";
    switch (type) {
      case TOKEN_TYPE::NUM:
        std::cout << "num: ";
        break;
      case TOKEN_TYPE::VAR:
        std::cout << "var ";
        break;
      case TOKEN_TYPE::UNARY_OP:
        std::cout << "u_op: ";
        break;
      case TOKEN_TYPE::BINARY_OP:
        std::cout << "b_op: ";
        break;
      case TOKEN_TYPE::BRA:
        std::cout << "bra: ";
        break;
      case TOKEN_TYPE::NONE:
        std::cout << "none ";
        break;
    }
    std::cout << value;
    std::cout << ")";
  }
  void printVal(){
    std::cout << value << ' ';
  }
};

#endif
