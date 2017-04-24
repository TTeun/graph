#include "equation/parser/parser.h"
#include <string>
#include <queue>
#include <stack>
#include <cmath>

using namespace std;

inline float add(float a, float b){
  return a + b;
}

inline float sub(float a, float b){
  return a - b;
}

inline float mul(float a, float b){
  return a * b;
}

inline float divide(float a, float b){
  if (b == 0.0f)
    cout << "Division by zero\n";
  return a / b;
}

float evaluateQueue(queue<Token> *q){
  stack<float> *stck = new stack<float>;

  Token current;
  typedef float (*f_bin)(float a, float b);
  static unordered_map<string, f_bin> bin_maps =
    {
      {"+", add},
      {"-", sub},
      {"*", mul},
      {"/", divide},
      {"^", pow}
    };
  f_bin binary_op;

  typedef float (*f_un)(float a);
  static unordered_map<string, f_un> un_maps =
    {
      {"sin", sin},
      {"cos", cos},
      {"tan", tan},
      {"exp", exp}
    };
  f_un unary_op;

  float a, b;
  while (not q->empty()){
    current = q->front();
    q->pop();
    if (current.type == TOKEN_TYPE::BRA)
      cout << "Mismatched parenthesis\n";

    if (current.type == TOKEN_TYPE::NUM)
      stck->push(stof(current.value));

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

  return stck->top();
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
    float f = evaluateQueue(q);
    cout << f << '\n';
    delete q;
  }
}
