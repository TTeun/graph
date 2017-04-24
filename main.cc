#include "equation/expression/expression.h"
#include "equation/parser/parser.h"
#include <string>
#include <queue>
#include <stack>
#include <cmath>

using namespace std;

int main(){
  string test;
  getline(cin, test);

  Expression *e(Parser::parse_input(&test));


  e->printVariables();
  // double f = e->evaluateQueue();
  // cout << f << '\n';

}
