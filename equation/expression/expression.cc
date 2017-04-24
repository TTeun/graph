#include "expression.h"
#include <stack>
#include <limits> // NaN
#include <unordered_map>

using namespace std;

Expression::Expression()
  : token_queue(new queue<Token>),
    variables(new vector<string>),
    m_state(new string())
{
}

Expression::Expression(queue<Token> *q, vector<string> *vrbls)
  : token_queue(q),
    variables(vrbls),
    m_state(new string())
{}

Expression::~Expression()
{
  delete token_queue;
  delete variables;
  delete m_state;
  // cout << "Expression destructor \n";
}

Expression::map_bin Expression::bin_maps =
  {
    {"+", [](double a, double b){return a + b;}},
    {"-", [](double a, double b){return a - b;}},
    {"*", [](double a, double b){return a * b;}},
    {"/", [](double a, double b){return a / b;}},
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

void Expression::printVariables()
{
  for (auto v : *variables)
    cout << v << '\n';
}

void Expression::evaluateQueue()
{
  if (not m_success)
    return;

  stack<double> *stck = new stack<double>;

  Token current;

  f_bin binary_op;
  f_un unary_op;

  double a, b;
  bool paren_mismatch = false;
  while (not token_queue->empty())
  {
    current = token_queue->front();
    token_queue->pop();
    if (current.type == TOKEN_TYPE::BRA)
    {
      paren_mismatch = true;
      break;
    }

    if (current.type == TOKEN_TYPE::NUM)
    {
      if (current.value == string("pi"))
        stck->push(PI);
      else
        stck->push(stod(current.value));
    }

    if (current.type == TOKEN_TYPE::BINARY_OP)
    {
      binary_op = bin_maps[current.value];
      b = stck->top();
      stck->pop();
      a = stck->top();
      stck->pop();
      stck->push(binary_op(a,b));
    }
    if (current.type == TOKEN_TYPE::UNARY_OP)
    {
      unary_op = un_maps[current.value];
      a = stck->top();
      stck->pop();
      stck->push(unary_op(a));
    }
  }

  if (paren_mismatch)
    setState(string("Mismatched parenthesis\n"));

  if (stck->size() != 1)
  {
    setState("Syntax error\n");
    m_value = numeric_limits<double>::quiet_NaN();
  }
  else
    m_value = stck->top();
  delete stck;
}

void Expression::printQueue()
{
  while (not token_queue->empty())
  {
    token_queue->front().printToken();
    token_queue->pop();
  }
  cout << "\n";
}

void Expression::setState(string && st)
{
  *m_state = st;
  m_success = false;
}
