#include "simplify.h"
#include "node_util.h"
#include <iostream>

using namespace std;

namespace simplify {

  void simplifyNode(Node *node){
    if (node_util::isNum(node) || node_util::isVar(node))
      return;

    if (node->token.value == string("+"))
      simplifyPlus(node);

    if (node->token.value == string("*"))
      simplifyMul(node);
  }

  void simplifyPlus(Node *node){
    if (node_util::isZero(node->left)){
      node_util::copyChild(node, node->right, node->left);
      return;
    }

    if (node_util::isZero(node->right)){
      node_util::copyChild(node, node->left, node->right);
      return;
    }
  }

  void simplifyMul(Node *node){
    if (node_util::isOne(node->left)){
      node_util::copyChild(node, node->right, node->left);
      return;
    }

    if (node_util::isOne(node->right)){
      node_util::copyChild(node, node->left, node->right);
      return;
    }
  }

}
