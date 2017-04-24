#include "expression.h"
#include <stack>
#include <unordered_map>

using namespace std;

Expression::Expression()
  : token_queue(new queue<Token>)
{}

Expression::Expression(queue<Token> *q, vector<string> *vrbls)
  : token_queue(q),
    variables(vrbls)
{}

Expression::~Expression(){
  delete token_queue;
  cout << "Expression destructor \n";
}

Expression::map_bin Expression::bin_maps =
  {
    {"+", add},
    {"-", sub},
    {"*", mul},
    {"/", divide},
    {"^", pow}
  };

Expression::map_un Expression::un_maps =
  {
    {"sin", sin},
    {"cos", cos},
    {"tan", tan},
    {"exp", exp}
  };

constexpr double Expression::PI;

void Expression::printVariables(){
  for (auto v : *variables)
    cout << v << '\n';
}

double Expression::evaluateQueue(){

  stack<double> *stck = new stack<double>;

  Token current;

  f_bin binary_op;
  f_un unary_op;

  double a, b;
  while (not token_queue->empty()){
    current = token_queue->front();
    token_queue->pop();
    if (current.type == TOKEN_TYPE::BRA)
      cout << "Mismatched parenthesis\n";

    if (current.type == TOKEN_TYPE::NUM){
      if (current.value == string("pi"))
        stck->push(PI);
      else
        stck->push(stod(current.value));
    }

    if (current.type == TOKEN_TYPE::BINARY_OP){
      binary_op = bin_maps[current.value];
      b = stck->top();
      stck->pop();
      a = stck->top();
      stck->pop();
      stck->push(binary_op(a,b));
    }
    if (current.type == TOKEN_TYPE::UNARY_OP){
      unary_op = un_maps[current.value];
      a = stck->top();
      stck->pop();
      stck->push(unary_op(a));
    }
  }
  double result = stck->top();
  delete stck;
  return result;
}

void Expression::printQueue(){
  while (not token_queue->empty()){
    token_queue->front().printToken();
    token_queue->pop();
  }
  cout << "\n";
}
