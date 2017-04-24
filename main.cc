#include "equation/parser/parser.h"
#include <string>
#include <queue>
#include <stack>
#include <cmath>

using namespace std;

inline double add(double a, double b){
  return a + b;
}

inline double sub(double a, double b){
  return a - b;
}

inline double mul(double a, double b){
  return a * b;
}

inline double divide(double a, double b){
  if (b == 0.0f)
    cout << "Division by zero\n";
  return a / b;
}

double evaluateQueue(queue<Token> *q){
  const double PI = 3.141592653589793;

  stack<double> *stck = new stack<double>;

  Token current;
  typedef double (*f_bin)(double a, double b);
  static unordered_map<string, f_bin> bin_maps =
    {
      {"+", add},
      {"-", sub},
      {"*", mul},
      {"/", divide},
      {"^", pow}
    };
  f_bin binary_op;

  typedef double (*f_un)(double a);
  static unordered_map<string, f_un> un_maps =
    {
      {"sin", sin},
      {"cos", cos},
      {"tan", tan},
      {"exp", exp}
    };
  f_un unary_op;

  double a, b;
  while (not q->empty()){
    current = q->front();
    q->pop();
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

void printQueue(queue<Token> *q){
  while (not q->empty()){
    q->front().printToken();
    q->pop();
  }
  cout << "\n";
}

int main(){
  string test;
  getline(cin, test);
  queue<Token> *q;
  q = Parser::parse_input(&test);
  if (q){
    // printQueue(q);
    double f = evaluateQueue(q);
    cout << f << '\n';
    delete q;
  }
}
