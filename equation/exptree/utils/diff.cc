#include "diff.h"
#include <iostream>
#include <assert.h>
#include <sstream>
#include "../../eqmaps/eqmaps.h"

using namespace std;
using namespace node_util;

namespace diff {

  typedef TOKEN_TYPE TT;

  // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  u_node diffPlus(Node const *node){
    return newNode(node->token, differentiateNode(node->right.get()), differentiateNode(node->left.get()));
  }

  // --------------------------------------------------------------------
  u_node diffMin(Node const *node){
    return newNode(Token(TT::BINARY_OP, string("-")), differentiateNode(node->right.get()), differentiateNode(node->left.get()));
  }

  // ********************************************************************
  u_node diffMul(Node const *node){
    auto left = newNode(Token(TT::BINARY_OP, string("*")), cpyNode(node->right.get()), differentiateNode(node->left.get()));
    auto right = newNode(Token(TT::BINARY_OP, string("*")), differentiateNode(node->right.get()), cpyNode(node->left.get()));
    return newNode(Token(TT::BINARY_OP, string("+")), right, left);
  }

  // /////////////////////////////////////////////////////////////////////
  u_node diffDiv(Node const *node){
    auto f_dash_g = newNode(Token(TT::BINARY_OP, string("*")), cpyNode(node->right.get()), differentiateNode(node->left.get()));
    auto g_dash_f = newNode(Token(TT::BINARY_OP, string("*")), differentiateNode(node->right.get()), cpyNode(node->left.get()));
    auto num      = newNode(Token(TT::BINARY_OP, string("-")), g_dash_f, f_dash_g);
    auto den      = newNode(Token(TT::BINARY_OP, string("^")), numNode(string("2")), cpyNode(node->right.get()));
    return newNode(Token(TT::BINARY_OP, string("/")), den, num);
  }

  // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  u_node diffPow(Node const *node){
    if (isNum(node->right.get())){
      auto left = numNode(node->right.get()->token.value);
      auto right = cpyNode(node);
      right->right->token.value = doubleToString(getNum(node->right.get()) - 1);
      return newNode(Token(TT::BINARY_OP, string("*")), right, left);
    }
    cout << "unimplemented power differentiation in diff.cc\n";
    return nullptr;
  }

  // sinsinsinsinsinsinsinsinsinsinsinsinsinsinsinsinsinsinsinsinsinsin
  u_node diffSin(Node const *node){
    auto new_node = newNode(Token(TT::BINARY_OP, string("*")), cpyNode(node), differentiateNode(node->right.get()));
    new_node->right->token.value = string("cos");
    return new_node;
  }

  // coscoscoscoscoscoscoscoscoscoscoscoscoscoscoscoscoscoscoscoscoscos
  u_node diffCos(Node const *node){
    auto sin_node = newNode(Token(TT::BINARY_OP, string("*")), cpyNode(node), differentiateNode(node->right.get()));
    sin_node->right->token.value = string("sin");
    return newNode(Token(TT::UNARY_OP, string("-u")), sin_node);
  }

  // expexpexpexpexpexpexpexpexpexpexpexpexpexpexpexpexpexpexpexpexpexp
  u_node diffExp(Node const *node){
    return newNode(Token(TT::BINARY_OP, string("*")), cpyNode(node), differentiateNode(node->right.get()));
  }

  // -u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u
  u_node diffUnaryMinus(Node const *node){
    auto new_node = make_unique<Node>(Token(TT::UNARY_OP, string("-u")));
    new_node->right = differentiateNode(node->right.get());
    return new_node;
  }

  // loglogloglogloglogloglogloglogloglogloglogloglogloglogloglogloglog
  u_node diffLog(Node const *node){
    auto right = newNode(Token(TT::BINARY_OP, string("/")), cpyNode(node->right.get()), numNode(string("1")));
    auto left = differentiateNode(node->right.get());
    return newNode(Token(TT::BINARY_OP, string("*")), right, left);
  }

  u_node differentiateNode(Node const *node){
    switch (node->token.type) {
      case TT::NUM:
        return numNode(string("0"));
        break;

      case TT::VAR:
      return numNode(string("1"));
        break;

      case TT::BINARY_OP:
        return EqMaps::bin_diff_maps[node->token.value](node);
        break;

      case TT::UNARY_OP:
        return EqMaps::bin_diff_maps[node->token.value](node);
        break;

      case TT::BRA:
        break;
      case TT::NONE:
        break;

      }
    cout << "Shouldn't get here! Missing derivative pattern in diff.cc\n";
    return nullptr;
  }

}
