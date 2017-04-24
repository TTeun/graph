#ifndef EXPRESSION__H
#define EXPRESSION__H

#include <queue>
#include <string>
#include "../token.h"
#include "../parser/parser.h"

class Expression {
  Expression();
  Expression(std::string *input);
  ~Expression();


private:
  std::queue<Token> *token_queue;
};

#endif
