#ifndef EXPRESSION__H
#define EXPRESSION__H

#include <queue>
#include <string>
#include <vector>
#include <stack>
#include <iostream>
#include <unordered_map>

#include "../token.h"
#include "../eqmaps/eqmaps.h"

class Expression {
public:
  Expression();
  Expression(std::queue<Token> *q, std::vector<std::string> *vrbls);
  ~Expression();

private:
  std::queue<Token> *token_queue;

  static double constexpr PI = 3.141592653589793;
  double toNum(std::string &str);
  bool binToNum(std::string &str, std::stack<double> *stck);

  std::vector<std::string> *variables;
  std::string *m_state;
  bool m_success = true;
  double m_value;

public:
  std::queue<Token> *q(){ return token_queue; }
  void setState(std::string && st);
  std::string *state();
  bool success();
  double value();
  void evaluateQueue();
  void printVariables();
  void printQueue();
};

#endif
