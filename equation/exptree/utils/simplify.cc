#include "simplify.h"
#include "node_util.h"
#include "../../eqmaps/eqmaps.h"
#include <iostream>
#include <assert.h>

using namespace std;
using namespace node_util;

namespace simplify {

  void simplifyNode(unique_ptr<Node> &node){
    if (isNum(node.get()) || isVar(node.get()))
      return;

    if (node->left)
      simplifyNode(node->left);

    if (node->right)
      simplifyNode(node->right);

    if (node->token.type == TOKEN_TYPE::BINARY_OP)
      simplifyBinaryOp(node);

  }

  void simplifyBinaryOp(unique_ptr<Node> &node){
    if (isNum(node->left.get()) && isNum(node->right.get())){
      node = unique_ptr<Node>(new Node(Token(TOKEN_TYPE::NUM, doubleToString(EqMaps::bin_maps[node->token.value](getNum(node->left.get()), getNum(node->right.get()))))));
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

    if (isZero(node->left.get())){
      node = move(node->right);
      return;
    }

    if (isZero(node->right.get())){
      node = move(node->left);
      return;
    }
  }

  void simplifyMul(unique_ptr<Node> &node){
    assert(node->token.value == string("*"));

    if (isOne(node->left.get())){
      node = move(node->right);
      return;
    }

    if (isOne(node->right.get())){
      node = move(node->left);
      return;
    }

    if (isZero(node->right.get()) || isZero(node->left.get())){
      node = unique_ptr<Node>(new Node(Token(TOKEN_TYPE::NUM, string("0"))));
      return;
    }
  }

  void simplifyPow(unique_ptr<Node> &node){
    assert(node->token.value == string("^"));
    assert(node->right->token.type == TOKEN_TYPE::NUM);
    if (isOne(node->right.get())){
      node = move(node->left);
      return;
    }

    if (isZero(node->right.get())){
      node = unique_ptr<Node>(new Node(Token(TOKEN_TYPE::NUM, string("1"))));
      return;
    }
  }
}
