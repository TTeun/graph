#include "simplify.h"
#include "node_util.h"
#include "../../eqmaps/eqmaps.h"
#include <iostream>
#include <assert.h>

using namespace std;
using namespace node_util;

namespace simplify {

  void simplifyNode(u_node &node){
    if (isNum(node.get()) || isVar(node.get()))
      return;

    if (node->left)
      simplifyNode(node->left);

    if (node->right)
      simplifyNode(node->right);

    if (node->token.type == TOKEN_TYPE::BINARY_OP)
      simplifyBinaryOp(node);

    if (node->token.type == TOKEN_TYPE::UNARY_OP)
      simplifyUnaryOp(node);

  }

  void simplifyBinaryOp(u_node &node){
    if (isNum(node->left.get()) && isNum(node->right.get())){
      node = numNode(doubleToString(EqMaps::bin_maps[node->token.value](getNum(node->left.get()), getNum(node->right.get()))));
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

    if (node->token.value == string("/")){
      simplifyDiv(node);
      return;
    }

  }

  void simplifyUnaryOp(u_node &node){
    if (isNum(node->right.get())){
      node = numNode(doubleToString(EqMaps::un_maps[node->token.value](getNum(node->right.get()))));
      return;
    }
  }

  void simplifyPlus(u_node &node){
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

  void simplifyMul(u_node &node){
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
      node = numNode(string("0"));
      return;
    }
  }

  void simplifyPow(u_node &node){
    assert(node->token.value == string("^"));
    assert(node->right->token.type == TOKEN_TYPE::NUM);
    if (isOne(node->right.get())){
      node = move(node->left);
      return;
    }

    if (isZero(node->right.get())){
      node = numNode(string("1"));
      return;
    }
  }

  void simplifyDiv(u_node &node){
    return;
  }
}
