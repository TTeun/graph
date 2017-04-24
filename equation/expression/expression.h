#ifndef EXPRESSION__H
#define EXPRESSION__H

#include <queue>
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <cmath>

#include "../token.h"

class Expression {
public:
  Expression();
  Expression(std::queue<Token> *q, std::vector<std::string> *vrbls);
  ~Expression();

private:
  std::queue<Token> *token_queue;

  typedef double (*f_bin)(double a, double b);
  typedef double (*f_un)(double a);
  typedef std::unordered_map<std::string, f_bin> map_bin;
  typedef std::unordered_map<std::string, f_un> map_un;

  static std::unordered_map<std::string, f_bin> bin_maps;
  static std::unordered_map<std::string, f_un> un_maps;
  // std::unordered_map<std::string, double> var_map;

  static double constexpr PI = 3.141592653589793;

  std::vector<std::string> *variables;
  std::string *m_state;
  bool m_success = true;
  double m_value;

public:
  void setState(std::string && st);
  std::string *state(){
    return m_state;
  }
  bool success(){
    return m_success;
  }
  double value(){
    return m_value;
  }
  void evaluateQueue();
  void printVariables();
  void printQueue();
};

#endif
