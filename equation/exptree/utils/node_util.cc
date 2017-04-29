#include "node_util.h"
#include <string>
#include <sstream>

using namespace std;

namespace node_util{

  Node *copyNode(Node *node){
    Node *new_node = new Node;
    if (node->left)
      new_node->left = copyNode(node->left);

    if (node->right)
      new_node->right = copyNode(node->right);

    new_node->token = node->token;
    return new_node;
  }

  Node *newNode(Token const &token, Node *right, Node *left){
    Node *new_node = new Node(token);
    new_node->left = left;
    new_node->right = right;
    return new_node;
  }

  void copyLeft(Node *node){
    node->token = node->left->token;
    Node *temp = node->left;
    delete node->right;
    node->right = node->left->right;
    node->left = node->left->left;
    delete temp;
  }

  void copyRight(Node *node){
    node->token = node->right->token;
    Node *temp = node->right;
    delete node->left;
    node->left = node->right->left;
    node->right = node->right->right;
    delete temp;
  }

  bool isZero(Node *node){
    return (node->token.type == TOKEN_TYPE::NUM && node->token.value == string("0"));
  }

  bool isOne(Node *node){
    return (node->token.type == TOKEN_TYPE::NUM && node->token.value == string("1"));
  }

  bool isVar(Node *node){
    return node->token.type == TOKEN_TYPE::VAR;
  }

  bool isNum(Node *node){
    return node->token.type == TOKEN_TYPE::NUM;
  }

  bool isTerminalNum(Node *node){
    return (not (node->left || node->right) && node->token.type == TOKEN_TYPE::NUM);
  }

  double getNum(Node *node){
    return stod(node->token.value);
  }

  void copyChild(Node *node, Node *child_to_copy, Node *other){
    delete other;

    node->token = child_to_copy->token;
    node->left  = child_to_copy->left;
    node->right = child_to_copy->right;

    delete child_to_copy;
    return;
  }

  void deleteChildren(Node *node){
    deleteTree(node->right);
    deleteTree(node->left);
    node->left = nullptr;
    node->right =nullptr;
  }

  void deleteTree(Node *node){
    if (node->left)
      deleteTree(node->left);

    if (node->right)
      deleteTree(node->right);

    delete node;
  }

  string doubleToString(double a){
    ostringstream ostr;
    ostr << a;
    return ostr.str();
  }

}
