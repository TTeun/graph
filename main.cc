#include "equation/expression/expression.h"
#include <string>
#include <queue>
#include <stack>
#include <cmath>

using namespace std;

int main(){
  string test;
  getline(cin, test);
  Expression *exp = new Expression(&test);
  double f = exp->evaluateQueue();
  cout << f << '\n';
  delete exp;

}
