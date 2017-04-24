#include "token.h"
#include <iostream>

using namespace std;

void Token::printToken(){
  cout << "(";
  switch (type) {
    case TOKEN_TYPE::NUM:
      cout << "num: ";
      break;
    case TOKEN_TYPE::VAR:
      cout << "var ";
      break;
    case TOKEN_TYPE::UNARY_OP:
      cout << "u_op: ";
      break;
    case TOKEN_TYPE::BINARY_OP:
      cout << "b_op: ";
      break;
    case TOKEN_TYPE::BRA:
      cout << "bra: ";
      break;
    case TOKEN_TYPE::NONE:
      cout << "none ";
      break;
  }
  cout << value;
  cout << ")";
}

void Token::printVal(){
  cout << value << ' ';
}
