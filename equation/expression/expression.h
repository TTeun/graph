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

  static double add(double a, double b);
  static double sub(double a, double b);
  static double mul(double a, double b);
  static double divide(double a, double b);

  typedef double (*f_bin)(double a, double b);
  typedef double (*f_un)(double a);
  typedef std::unordered_map<std::string, f_bin> map_bin;
  typedef std::unordered_map<std::string, f_un> map_un;

  static std::unordered_map<std::string, f_bin> bin_maps;
  static std::unordered_map<std::string, f_un> un_maps;

  static double constexpr PI = 3.141592653589793;

  std::vector<std::string> *variables;
public:
  double evaluateQueue();
  void printVariables();
  void printQueue();
};

inline double Expression::add(double a, double b){
  return a + b;
}

inline double Expression::sub(double a, double b){
  return a - b;
}

inline double Expression::mul(double a, double b){
  return a * b;
}

inline double Expression::divide(double a, double b){
  if (b == 0.0)
    std::cout << "Division by zero\n";
  return a / b;
}

#endif
