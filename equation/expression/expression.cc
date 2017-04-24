#include "expression.h"
#include <cmath>

using namespace std;

Expression::Expression()
  : token_queue(new queue<Token>)
{}

Expression::Expression(string *input)
  : token_queue(Parser::parse_input(input))
{}

Expression::~Expression(){
  delete token_queue;
}
