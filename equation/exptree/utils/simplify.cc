#include "simplify.h"
#include "node_util.h"
#include "../../eqmaps/eqmaps.h"
#include <iostream>
#include <assert.h>

using namespace std;

namespace simplify {

  void simplifyNode(Node *node){
    if (node_util::isNum(node) || node_util::isVar(node))
      return;

    if (node->left)
      simplifyNode(node->left);

    if (node->right)
      simplifyNode(node->right);

    if (node->token.type == TOKEN_TYPE::BINARY_OP)
      simplifyBinaryOp(node);

  }

  void simplifyBinaryOp(Node *node){
    if (node_util::isNum(node->left) && node_util::isNum(node->right)){
      float a = node_util::getNum(node->right);
      float b = node_util::getNum(node->left);
      string res = node_util::doubleToString(EqMaps::bin_maps[node->token.value](b, a));
      node->token = Token(TOKEN_TYPE::NUM, res);
      node_util::deleteChildren(node);
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

    if (node_util::isZero(node->left)){
      node_util::copyChild(node, node->right, node->left);
      return;
    }

    if (node_util::isZero(node->right)){
      node_util::copyChild(node, node->left, node->right);
      return;
    }
    return;
  }

  void simplifyMul(Node *node){
    assert(node->token.value == string("*"));

    if (node_util::isOne(node->left)){
      node_util::copyChild(node, node->right, node->left);
      return;
    }

    if (node_util::isOne(node->right)){
      node_util::copyChild(node, node->left, node->right);
      return;
    }

    if (node_util::isZero(node->right) || node_util::isZero(node->left)){
      node_util::deleteChildren(node);
      node->token = Token(TOKEN_TYPE::NUM, string("0"));
      return;
    }
    return;
  }

  void simplifyPow(Node *node){
    assert(node->token.value == string("^"));
    assert(node->right->token.type == TOKEN_TYPE::NUM);
    if (node_util::isOne(node->right)){
      node_util::copyChild(node, node->left, node->right);
      return;
    }

    if (node_util::isZero(node->right)){
      node_util::deleteChildren(node);
      node->token = Token(TOKEN_TYPE::NUM, string("1"));
      return;
    }
    return;
  }
}
