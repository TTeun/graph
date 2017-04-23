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

inline float sinf(float a){
  return sin(a);
}

float evaluateQueue(queue<Token> *q){
  stack<float> *stck = new stack<float>;

  Token current;
  float (*f)(float a, float b);
  float a, b;
  while (not q->empty()){
    current = q->front();
    q->pop();
    if (current.type == TOKEN_TYPE::BRA)
      cout << "Mismatched parenthesis\n";

    if (current.type == TOKEN_TYPE::NUM)
      stck->push(stof(current.value));

    if (current.type == TOKEN_TYPE::OP){
      if (current.value == string("+"))
        f = add;
      else if (current.value == string("-"))
        f = sub;
      else if (current.value == string("*"))
        f = mul;
      else if (current.value == string("/"))
        f = divide;
      else if (current.value == string("sin"))
        f = sinf;

      b = stck->top();
      stck->pop();
      a = stck->top();
      stck->pop();
      stck->push(f(a,b));
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
    float f = evaluateQueue(q);
    cout << f << '\n';
    delete q;
  }
}
