#include "node_util.h"
#include <string>
#include <sstream>

using namespace std;

namespace node_util{

  unique_ptr<Node> newNode(Token const &token, unique_ptr<Node> &right, unique_ptr<Node> &left){
    unique_ptr<Node> new_node = unique_ptr<Node>(new Node(token));
    new_node->left = move(left);
    new_node->right = move(right);
    return new_node;
  }

  unique_ptr<Node> newNode(Token const &token, unique_ptr<Node> &&right, unique_ptr<Node> &&left){
    unique_ptr<Node> new_node = unique_ptr<Node>(new Node(token));
    new_node->left = move(left);
    new_node->right = move(right);
    return new_node;
  }

  unique_ptr<Node> newNode(Token const &token, unique_ptr<Node> &right){
    unique_ptr<Node> new_node = unique_ptr<Node>(new Node(token));
    new_node->right = move(right);
    return new_node;
  }

  unique_ptr<Node> newNode(Token const &token, unique_ptr<Node> &&right){
    unique_ptr<Node> new_node = unique_ptr<Node>(new Node(token));
    new_node->right = move(right);
    return new_node;
  }

  unique_ptr<Node> cpyNode(unique_ptr<Node> &node){
    unique_ptr<Node> new_node(new Node(node->token));
    if (node->left)
      new_node->left = cpyNode(node->left);

    if (node->right)
      new_node->right = cpyNode(node->right);

    return new_node;
  }

  bool isZero(unique_ptr<Node> &node){
    return (node->token.type == TOKEN_TYPE::NUM && node->token.value == string("0"));
  }

  bool isOne(unique_ptr<Node> &node){
    return (node->token.type == TOKEN_TYPE::NUM && node->token.value == string("1"));
  }

  bool isVar(unique_ptr<Node> &node){
    return node->token.type == TOKEN_TYPE::VAR;
  }

  bool isNum(unique_ptr<Node> &node){
    return node->token.type == TOKEN_TYPE::NUM;
  }

  bool isTerminalNum(unique_ptr<Node> &node){
    return (not (node->left || node->right) && node->token.type == TOKEN_TYPE::NUM);
  }

  double getNum(unique_ptr<Node> &node){
    return stod(node->token.value);
  }

  string doubleToString(double a){
    ostringstream ostr;
    ostr << a;
    return ostr.str();
  }

}
