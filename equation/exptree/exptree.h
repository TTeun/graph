#ifndef TREE__H
#define TREE__H

#include "../expression/expression.h"
#include <vector>

class ExpTree {
public:
  ExpTree();
  ExpTree(std::queue<Token> *token_queue);
  ~ExpTree();

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
  Node *dif_tree = nullptr;
  void clearNode(Node *node);
  void printTreeInternal(Node *node);
  void simplifyNode(Node *node);
  bool isTerminalNum(Node *node);
  double getNum(Node *node);
  bool isZero(Node *node);
  bool isOne(Node *node);
  bool isVar(Node *node);
  bool isNum(Node *node);
  bool isVarExp(Node *node);
  bool isSumNode(Node *node);
  void getSumNodes(Node *node, std::vector<Node*> *sumNodes);
  void cleanChildren(Node *node);
  void copyRight(Node *node);
  void copyLeft(Node *node);

  void checkEasySimplify(Node* node);
  Node* copyNode(Node *node);
  Node* differentiateNode(Node *node);

public:
  void printTree();
  void printDif();
  void simplify();
  void differentiate();

};

#endif
