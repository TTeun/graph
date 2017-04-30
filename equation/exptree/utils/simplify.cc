#include "simplify.h"
#include "node_util.h"
#include "../../eqmaps/eqmaps.h"
#include <iostream>
#include <assert.h>

using namespace std;
using namespace node_util;

namespace simplify {

  void simplifyNode(unique_ptr<Node> &node){
    if (isNum(node) || isVar(node))
      return;

    if (node->left)
      simplifyNode(node->left);

    if (node->right)
      simplifyNode(node->right);

    if (node->token.type == TOKEN_TYPE::BINARY_OP)
      simplifyBinaryOp(node);

  }

  void simplifyBinaryOp(unique_ptr<Node> &node){
    if (isNum(node->left) && isNum(node->right)){
      node = unique_ptr<Node>(new Node(Token(TOKEN_TYPE::NUM, doubleToString(EqMaps::bin_maps[node->token.value](getNum(node->left), getNum(node->right))))));
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

  void simplifyPlus(unique_ptr<Node> &node){
    assert(node->token.value == string("+"));

    if (isZero(node->left)){
      node = move(node->right);
      return;
    }

    if (isZero(node->right)){
      node = move(node->left);
      return;
    }
  }

  void simplifyMul(unique_ptr<Node> &node){
    assert(node->token.value == string("*"));

    if (isOne(node->left)){
      node = move(node->right);
      return;
    }

    if (isOne(node->right)){
      node = move(node->left);
      return;
    }

    if (isZero(node->right) || isZero(node->left)){
      node = unique_ptr<Node>(new Node(Token(TOKEN_TYPE::NUM, string("0"))));
      return;
    }
  }

  void simplifyPow(unique_ptr<Node> &node){
    assert(node->token.value == string("^"));
    assert(node->right->token.type == TOKEN_TYPE::NUM);
    if (isOne(node->right)){
      node = move(node->left);
      return;
    }

    if (isZero(node->right)){
      node = unique_ptr<Node>(new Node(Token(TOKEN_TYPE::NUM, string("1"))));
      return;
    }
  }
}
