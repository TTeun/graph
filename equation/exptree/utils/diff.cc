#include "diff.h"
#include <iostream>
#include <assert.h>
#include <sstream>
#include "../../eqmaps/eqmaps.h"

using namespace std;
using namespace node_util;

namespace diff {
  // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  unique_ptr<Node> diffPlus(Node const *node){
    auto ptr = newNode(node->token, differentiateNode(node->right.get()), differentiateNode(node->left.get()));
    return ptr;
  }

  // --------------------------------------------------------------------
  unique_ptr<Node> diffMin(Node const *node){
    return newNode(Token(TOKEN_TYPE::BINARY_OP, string("-")), differentiateNode(node->right.get()), differentiateNode(node->left.get()));
  }

  // ********************************************************************
  unique_ptr<Node> diffMul(Node const *node){
    auto left = newNode(Token(TOKEN_TYPE::BINARY_OP, string("*")), cpyNode(node->right.get()), differentiateNode(node->left.get()));
    auto right = newNode(Token(TOKEN_TYPE::BINARY_OP, string("*")), differentiateNode(node->right.get()), cpyNode(node->left.get()));
    return newNode(Token(TOKEN_TYPE::BINARY_OP, string("+")), right, left);
  }

  // /////////////////////////////////////////////////////////////////////
  unique_ptr<Node> diffDiv(Node const *node){
    auto f_dash_g = newNode(Token(TOKEN_TYPE::BINARY_OP, string("*")), cpyNode(node->right.get()), differentiateNode(node->left.get()));
    auto g_dash_f = newNode(Token(TOKEN_TYPE::BINARY_OP, string("*")), differentiateNode(node->right.get()), cpyNode(node->left.get()));
    auto num      = newNode(Token(TOKEN_TYPE::BINARY_OP, string("-")), g_dash_f, f_dash_g);
    auto den      = newNode(Token(TOKEN_TYPE::BINARY_OP, string("^")), numNode(string("2")), cpyNode(node->right.get()));
    return newNode(Token(TOKEN_TYPE::BINARY_OP, string("/")), den, num);
  }

  // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  unique_ptr<Node> diffPow(Node const *node){
    if (isNum(node->right.get())){
      auto left = make_unique<Node>(TOKEN_TYPE::NUM, doubleToString(getNum(node->right.get())));
      auto right = cpyNode(node);
      right->right->token.value = doubleToString(getNum(node->right.get()) - 1);
      return newNode(Token(TOKEN_TYPE::BINARY_OP, string("*")), right, left);
    }
    cout << "unimplemented power differentiation in diff.cc\n";
    return nullptr;
  }

  // sinsinsinsinsinsinsinsinsinsinsinsinsinsinsinsinsinsinsinsinsinsin
  unique_ptr<Node> diffSin(Node const *node){
    auto new_node = newNode(Token(TOKEN_TYPE::BINARY_OP, string("*")), cpyNode(node), differentiateNode(node->right.get()));
    new_node->right->token.value = string("cos");
    return new_node;
  }

  // coscoscoscoscoscoscoscoscoscoscoscoscoscoscoscoscoscoscoscoscoscos
  unique_ptr<Node> diffCos(Node const *node){
    auto sin_node = newNode(Token(TOKEN_TYPE::BINARY_OP, string("*")), cpyNode(node), differentiateNode(node->right.get()));
    sin_node->right->token.value = string("sin");
    return newNode(Token(TOKEN_TYPE::UNARY_OP, string("-u")), sin_node);
  }

  // expexpexpexpexpexpexpexpexpexpexpexpexpexpexpexpexpexpexpexpexpexp
  unique_ptr<Node> diffExp(Node const *node){
    return newNode(Token(TOKEN_TYPE::BINARY_OP, string("*")), cpyNode(node), differentiateNode(node->right.get()));
  }

  // -u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u
  unique_ptr<Node> diffUnaryMinus(Node const *node){
    auto new_node = make_unique<Node>(Token(TOKEN_TYPE::UNARY_OP, string("-u")));
    new_node->right = differentiateNode(node->right.get());
    return new_node;
  }

  // loglogloglogloglogloglogloglogloglogloglogloglogloglogloglogloglog
  unique_ptr<Node> diffLog(Node const *node){
    auto right = newNode(Token(TOKEN_TYPE::BINARY_OP, string("/")), cpyNode(node->right.get()), numNode(string("1")));
    auto left = differentiateNode(node->right.get());
    return newNode(Token(TOKEN_TYPE::BINARY_OP, string("*")), right, left);
  }

  unique_ptr<Node> numNode(string &&str){
    return newNode(Token(TOKEN_TYPE::NUM, str), nullptr, nullptr);
  }

  unique_ptr<Node> differentiateNode(Node const *node){
    switch (node->token.type) {
      case TOKEN_TYPE::NUM:
        return unique_ptr<Node>(make_unique<Node>(Token(TOKEN_TYPE::NUM, string("0"))));
        break;

      case TOKEN_TYPE::VAR:
        return unique_ptr<Node>(make_unique<Node>(Token(TOKEN_TYPE::NUM, string("1"))));
        break;

      case TOKEN_TYPE::BINARY_OP:
        return EqMaps::bin_diff_maps[node->token.value](node);
        break;

      case TOKEN_TYPE::UNARY_OP:
        return EqMaps::bin_diff_maps[node->token.value](node);
        break;

      case TOKEN_TYPE::BRA:
        break;
      case TOKEN_TYPE::NONE:
        break;

      }
    cout << "Shouldn't get here! Missing derivative pattern in diff.cc\n";
    return nullptr;
  }

}
