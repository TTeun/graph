#include "expression.h"
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
    {"exp", exp},
    {"-u", [](double a){return -a;}}
  };

constexpr double Expression::PI;

void Expression::printVariables()
{
  for (auto v : *variables)
    cout << v << '\n';
}

double Expression::toNum(string &str){
  return str == string("pi") ? PI : stod(str);
}

bool Expression::binToNum(string &str, stack<double> *stck)
{
  if (stck->size() < 2)
    return false;

  double a, b;
  b = stck->top();
  stck->pop();
  a = stck->top();
  stck->pop();
  stck->push(bin_maps[str](a,b));
  return true;
}

void Expression::evaluateQueue()
{
  // There was a parser error, evaluating is useless
  if (not m_success)
    return;

  stack<double> *stck = new stack<double>;
  Token current;
  double a;
  bool paren_mismatch = false;
  bool syntax_error = false;
  while (not token_queue->empty())
  {
    current = token_queue->front();
    token_queue->pop();
    switch (current.type) {
      case TOKEN_TYPE::BRA:
        paren_mismatch = true;
        break;
      case TOKEN_TYPE::NUM:
        stck->push(toNum(current.value));
        break;
      case TOKEN_TYPE::BINARY_OP:
        syntax_error = not binToNum(current.value, stck);
        break;
      case TOKEN_TYPE::UNARY_OP:
        if (stck->size() < 1){
          syntax_error = true;
          break;
        }
        a = stck->top();
        stck->pop();
        stck->push(un_maps[current.value](a));
        break;
      default:
        break;
    }
  }

  if (paren_mismatch)
    setState(string("Mismatched parenthesis\n"));

  if (stck->size() != 1 || syntax_error)
    setState("Syntax error\n");

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
  m_value = numeric_limits<double>::quiet_NaN();
}

string *Expression::state(){
  return m_state;
}
bool Expression::success(){
  return m_success;
}
double Expression::value(){
  return m_value;
}
