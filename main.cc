#include "equation/expression/expression.h"
#include "equation/exptree/exptree.h"
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


  ExpTree *exp_tree = new ExpTree(e->q());
  exp_tree->printTree();
}
