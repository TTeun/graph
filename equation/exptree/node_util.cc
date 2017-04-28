#include "node_util.h"

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

  bool isNum(Node *node){
    return node->token.type == TOKEN_TYPE::NUM;
  }

  double getNum(Node *node){
    return stod(node->token.value);
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

}
