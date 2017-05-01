#include "node_util.h"
#include <string>
#include <sstream>

using namespace std;

namespace node_util{

  u_node newNode(Token const &token, u_node &right, u_node &left){
    auto new_node = make_unique<Node>(token);
    new_node->left = move(left);
    new_node->right = move(right);
    return new_node;
  }

  u_node newNode(Token const &token, u_node &&right, u_node &&left){
    auto new_node = make_unique<Node>(token);
    new_node->left = move(left);
    new_node->right = move(right);
    return new_node;
  }

  u_node newNode(Token const &token, u_node &right){
    auto new_node = make_unique<Node>(token);
    new_node->right = move(right);
    return new_node;
  }

  u_node newNode(Token const &token, u_node &&right){
    auto new_node = make_unique<Node>(token);
    new_node->right = move(right);
    return new_node;
  }

  u_node cpyNode(Node const *node){
    auto new_node(make_unique<Node>(node->token));
    if (node->left)
      new_node->left = cpyNode(node->left.get());

    if (node->right)
      new_node->right = cpyNode(node->right.get());

    return new_node;
  }

  bool isZero(Node const *node){
    return (node->token.type == TOKEN_TYPE::NUM && node->token.value == string("0"));
  }

  bool isOne(Node const *node){
    return (node->token.type == TOKEN_TYPE::NUM && node->token.value == string("1"));
  }

  bool isVar(Node const *node){
    return node->token.type == TOKEN_TYPE::VAR;
  }

  bool isNum(Node const *node){
    return node->token.type == TOKEN_TYPE::NUM;
  }

  bool isTerminalNum(Node const *node){
    return (not (node->left || node->right) && node->token.type == TOKEN_TYPE::NUM);
  }

  double getNum(Node const *node){
    return stod(node->token.value);
  }

  string doubleToString(double a){
    ostringstream ostr;
    ostr << a;
    return ostr.str();
  }

}
