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
  e->evaluateQueue();
  if (not e->success())
    cout << *(e->state());
  else {
    e->printVariables();
    cout << e->value() << '\n';
  }
  delete e;

}
