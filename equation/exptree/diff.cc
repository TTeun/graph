#include "diff.h"
#include <iostream>
#include <sstream>
#include "../eqmaps/eqmaps.h"

using namespace std;

namespace diff{
  // +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Node* diffPlus(Node *node){
    return node_util::newNode(node->token, differentiateNode(node->right), differentiateNode(node->left));
  }

  // --------------------------------------------------------------------
  Node* diffMin(Node *node){
    return node_util::newNode(Token(TOKEN_TYPE::BINARY_OP, string("-")), differentiateNode(node->right), differentiateNode(node->left));
  }

  // ********************************************************************
  Node* diffMul(Node *node){
    Node *left = node_util::newNode(Token(TOKEN_TYPE::BINARY_OP, string("*")), node_util::copyNode(node->right), differentiateNode(node->left));
    Node *right = node_util::newNode(Token(TOKEN_TYPE::BINARY_OP, string("*")), differentiateNode(node->right), node_util::copyNode(node->left));
    return node_util::newNode(Token(TOKEN_TYPE::BINARY_OP, string("+")), right, left);
  }

  // /////////////////////////////////////////////////////////////////////
  Node *diffDiv(Node *node){
    Node *f_dash_g = node_util::newNode(Token(TOKEN_TYPE::BINARY_OP, string("*")), node_util::copyNode(node->right), differentiateNode(node->left));
    Node *g_dash_f = node_util::newNode(Token(TOKEN_TYPE::BINARY_OP, string("*")), differentiateNode(node->right), node_util::copyNode(node->left));
    Node *num      = node_util::newNode(Token(TOKEN_TYPE::BINARY_OP, string("-")), g_dash_f, f_dash_g);
    Node *den      = node_util::newNode(Token(TOKEN_TYPE::BINARY_OP, string("^")), numNode(string("2")), node_util::copyNode(node->right));
    return           node_util::newNode(Token(TOKEN_TYPE::BINARY_OP, string("/")), den, num);
  }

  // ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  Node* diffPow(Node *node){
    if (node_util::isNum(node->right)){
      ostringstream str1, str2;
      str1 << node_util::getNum(node->right) - 1;
      str2 << node_util::getNum(node->right);

      Node *right = node_util::copyNode(node);
      right->right->token.value = str1.str();

      return node_util::newNode(Token(TOKEN_TYPE::BINARY_OP, string("*")), right, node_util::newNode(Token(TOKEN_TYPE::NUM, str2.str()), nullptr, nullptr));
    }
    return nullptr;
  }


  // sinsinsinsinsinsinsinsinsinsinsinsinsinsinsinsinsinsinsinsinsinsin
  Node *diffSin(Node *node){
    Node *new_node = node_util::newNode(Token(TOKEN_TYPE::BINARY_OP, string("*")), node_util::copyNode(node), differentiateNode(node->right));
    new_node->right->token.value = string("cos");
    return new_node;
  }

  // coscoscoscoscoscoscoscoscoscoscoscoscoscoscoscoscoscoscoscoscoscos
  Node *diffCos(Node *node){
    Node *sin_node = node_util::newNode(Token(TOKEN_TYPE::BINARY_OP, string("*")), node_util::copyNode(node), differentiateNode(node->right));
    sin_node->right->token.value = string("sin");

    return node_util::newNode(Token(TOKEN_TYPE::UNARY_OP, string("-u")), sin_node);
  }

  // expexpexpexpexpexpexpexpexpexpexpexpexpexpexpexpexpexpexpexpexpexp
  Node* diffExp(Node *node){
    return node_util::newNode(Token(TOKEN_TYPE::BINARY_OP, string("*")), node_util::copyNode(node), differentiateNode(node->right));
  }

  // -u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u-u
  Node* diffUnaryMinus(Node *node){
    Node *new_node = new Node(Token(TOKEN_TYPE::UNARY_OP, string("-u")));
    new_node = differentiateNode(node->right);
    return new_node;
  }

  // loglogloglogloglogloglogloglogloglogloglogloglogloglogloglogloglog
  Node *diffLog(Node *node){
    Node* right = node_util::newNode(Token(TOKEN_TYPE::BINARY_OP, string("/")), node_util::copyNode(node->right), numNode(string("1")));
    return node_util::newNode(Token(TOKEN_TYPE::BINARY_OP, string("*")), right, differentiateNode(node->right));
  }

  Node *numNode(string &&str){
    return node_util::newNode(Token(TOKEN_TYPE::NUM, str), nullptr, nullptr);
  }

  Node *differentiateNode(Node *node){
    switch (node->token.type) {
      case TOKEN_TYPE::NUM:
        return new Node(Token(TOKEN_TYPE::NUM, string("0")));
        break;

      case TOKEN_TYPE::VAR:
        return new Node(Token(TOKEN_TYPE::VAR, string("1")));
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
    cout << "Shouldn't get here! Missing derivative pattern in exptree.cc\n";
    return nullptr;
  }

}
