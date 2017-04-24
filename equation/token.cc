#include "token.h"
#include <iostream>

void Token::printToken(){
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

void Token::printVal(){
  std::cout << value << ' ';
}
