#include "diff.h"
#include <iostream>
#include <assert.h>
#include <sstream>
#include "../../eqmaps/eqmaps.h"

using namespace std;
using namespace node_util;

namespace diff {
  // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  unique_ptr<Node> diffPlus(unique_ptr<Node> &node){

    unique_ptr<Node> ptr = newNode(node->token, differentiateNode(node->right), differentiateNode(node->left));
    return ptr;
  }

  // --------------------------------------------------------------------
  unique_ptr<Node> diffMin(unique_ptr<Node> &node){
    return newNode(Token(TOKEN_TYPE::BINARY_OP, string("-")), differentiateNode(node->right), differentiateNode(node->left));
  }

  // ********************************************************************
  unique_ptr<Node> diffMul(unique_ptr<Node> &node){
    unique_ptr<Node> left = newNode(Token(TOKEN_TYPE::BINARY_OP, string("*")), cpyNode(node->right), differentiateNode(node->left));
    unique_ptr<Node> right = newNode(Token(TOKEN_TYPE::BINARY_OP, string("*")), differentiateNode(node->right), cpyNode(node->left));
    return newNode(Token(TOKEN_TYPE::BINARY_OP, string("+")), right, left);
  }

  // /////////////////////////////////////////////////////////////////////
  unique_ptr<Node> diffDiv(unique_ptr<Node> &node){
    unique_ptr<Node> f_dash_g = newNode(Token(TOKEN_TYPE::BINARY_OP, string("*")), cpyNode(node->right), differentiateNode(node->left));
    unique_ptr<Node> g_dash_f = newNode(Token(TOKEN_TYPE::BINARY_OP, string("*")), differentiateNode(node->right), cpyNode(node->left));
    unique_ptr<Node> num      = newNode(Token(TOKEN_TYPE::BINARY_OP, string("-")), g_dash_f, f_dash_g);
    unique_ptr<Node> den      = newNode(Token(TOKEN_TYPE::BINARY_OP, string("^")), numNode(string("2")), cpyNode(node->right));
    return newNode(Token(TOKEN_TYPE::BINARY_OP, string("/")), den, num);
  }

  // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  unique_ptr<Node> diffPow(unique_ptr<Node> &node){
    if (isNum(node->right)){
      unique_ptr<Node> right = cpyNode(node);
      right->right->token.value = doubleToString(getNum(node->right) - 1);
      unique_ptr<Node> new_node(new Node(Token(TOKEN_TYPE::BINARY_OP, string("*"))));
      new_node->right = move(right);
      unique_ptr<Node> left(new Node(TOKEN_TYPE::BINARY_OP, doubleToString(getNum(node->right))));
      new_node->left = move(left);
      return new_node;
    }
    return nullptr;
  }

  // sinsinsinsinsinsinsinsinsinsinsinsinsinsinsinsinsinsinsinsinsinsin
  unique_ptr<Node> diffSin(unique_ptr<Node> &node){
    unique_ptr<Node> new_node = newNode(Token(TOKEN_TYPE::BINARY_OP, string("*")), cpyNode(node), differentiateNode(node->right));
    new_node->right->token.value = string("cos");
    return new_node;
  }

  // coscoscoscoscoscoscoscoscoscoscoscoscoscoscoscoscoscoscoscoscoscos
  unique_ptr<Node> diffCos(unique_ptr<Node> &node){
    unique_ptr<Node> sin_node = newNode(Token(TOKEN_TYPE::BINARY_OP, string("*")), cpyNode(node), differentiateNode(node->right));
    sin_node->right->token.value = string("sin");

    return newNode(Token(TOKEN_TYPE::UNARY_OP, string("-u")), sin_node);
  }

  // expexpexpexpexpexpexpexpexpexpexpexpexpexpexpexpexpexpexpexpexpexp
  unique_ptr<Node> diffExp(unique_ptr<Node> &node){
    return newNode(Token(TOKEN_TYPE::BINARY_OP, string("*")), cpyNode(node), differentiateNode(node->right));
  }

  // -u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u
  unique_ptr<Node> diffUnaryMinus(unique_ptr<Node> &node){
    unique_ptr<Node> new_node(new Node(Token(TOKEN_TYPE::UNARY_OP, string("-u"))));
    new_node = differentiateNode(node->right);
    return new_node;
  }

  // loglogloglogloglogloglogloglogloglogloglogloglogloglogloglogloglog
  unique_ptr<Node> diffLog(unique_ptr<Node> &node){
    unique_ptr<Node> right = newNode(Token(TOKEN_TYPE::BINARY_OP, string("/")), cpyNode(node->right), numNode(string("1")));
    unique_ptr<Node> left = differentiateNode(node->right);
    return newNode(Token(TOKEN_TYPE::BINARY_OP, string("*")), right, left);
  }

  unique_ptr<Node> numNode(string &&str){
    return newNode(Token(TOKEN_TYPE::NUM, str), nullptr, nullptr);
  }

  unique_ptr<Node> differentiateNode(unique_ptr<Node> &node){
    switch (node->token.type) {
      case TOKEN_TYPE::NUM:
        return unique_ptr<Node>(new Node(Token(TOKEN_TYPE::NUM, string("0"))));
        break;

      case TOKEN_TYPE::VAR:
        return unique_ptr<Node>(new Node(Token(TOKEN_TYPE::NUM, string("1"))));
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
