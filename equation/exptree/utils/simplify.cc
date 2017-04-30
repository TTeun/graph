#include "simplify.h"
#include "node_util.h"
#include "../../eqmaps/eqmaps.h"
#include <iostream>
#include <assert.h>

using namespace std;
using namespace node_util;

namespace simplify {

  void simplifyNode(Node *node){
    if (isNum(node) || isVar(node))
      return;

    if (node->left)
      simplifyNode(node->left);

    if (node->right)
      simplifyNode(node->right);

    if (node->token.type == TOKEN_TYPE::BINARY_OP)
      simplifyBinaryOp(node);

  }

  void simplifyBinaryOp(Node *node){
    if (isNum(node->left) && isNum(node->right)){
      string res = doubleToString(EqMaps::bin_maps[node->token.value](getNum(node->left), getNum(node->right)));
      node->token = Token(TOKEN_TYPE::NUM, res);
      deleteChildren(node);
      return;
    }

    if (node->token.value == string("+")){
      simplifyPlus(node);
      return;
    }

    if (node->token.value == string("*")){
      simplifyMul(node);
      return;
    }

    if (node->token.value == string("^")){
      simplifyPow(node);
      return;
    }
  }

  void simplifyPlus(Node *node){
    assert(node->token.value == string("+"));

    if (isZero(node->left)){
      copyChild(node, node->right, node->left);
      return;
    }

    if (isZero(node->right)){
      copyChild(node, node->left, node->right);
      return;
    }
  }

  void simplifyMul(Node *node){
    assert(node->token.value == string("*"));

    if (isOne(node->left)){
      copyChild(node, node->right, node->left);
      return;
    }

    if (isOne(node->right)){
      copyChild(node, node->left, node->right);
      return;
    }

    if (isZero(node->right) || isZero(node->left)){
      deleteChildren(node);
      node->token = Token(TOKEN_TYPE::NUM, string("0"));
      return;
    }
  }

  void simplifyPow(Node *node){
    assert(node->token.value == string("^"));
    assert(node->right->token.type == TOKEN_TYPE::NUM);
    if (isOne(node->right)){
      copyChild(node, node->left, node->right);
      return;
    }

    if (isZero(node->right)){
      deleteChildren(node);
      node->token = Token(TOKEN_TYPE::NUM, string("1"));
      return;
    }
  }
}
