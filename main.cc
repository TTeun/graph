// #include "equation/expression/expression.h"
#include "equation/exptree/exptree.h"
#include "equation/parser/parser.h"
#include <string>
#include <iostream>

using namespace std;

int main(){

  cout << "Welcome to the diff tool!\n";

  string test;
  getline(cin, test);
  ExpTree *exp_tree = new ExpTree;
  while (test != string("q")){
    switch (test[0]) {
      case 'e':
        getline(cin, test);
        exp_tree->clearTree();
        exp_tree->setTree(Parser::parse_input(&test));
        exp_tree->printTree();
        getline(cin, test);
        break;
      case 'd':
        exp_tree->differentiate();
        exp_tree->printDif();
        getline(cin, test);
        break;
      default:
        cout << "No command\n";
        getline(cin, test);
        break;
    }
  }
  delete exp_tree;
}
