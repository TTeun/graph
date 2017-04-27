#ifndef TREE__H
#define TREE__H

#include "../expression/expression.h"


class ExpTree {
public:
  ExpTree();
  ExpTree(std::queue<Token> *token_queue);
  ~ExpTree();
  void printTree();

private:
  struct Node{
    Node(){}
    Node(Token &_token){
      token = _token;
    }

    Token token;
    Node *left = nullptr;
    Node *right = nullptr;
  };
  Node *exp_tree = nullptr;
  void clearNode(Node *node);
  void printTreeInternal(Node *node);
  void simplifyNode(Node *node);
  bool isTerminalNum(Node *node);
  double getNum(Node *node);
public:
  void simplify();

};

#endif
