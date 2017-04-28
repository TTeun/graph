#ifndef EXPTREE__H
#define EXPTREE__H

// #include "../expression/expression.h"
#include <queue>
#include <vector>
#include "../token.h"

class ExpTree {
public:
  ExpTree();
  ExpTree(std::queue<Token> *token_queue);
  ~ExpTree();

  struct Node{
    Node(){}
    Node(Token const &_token){
      token = _token;
    }
    Node(Token &&_token){
      token = _token;
    }

    Token token;
    Node *left = nullptr;
    Node *right = nullptr;
  };
private:
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
  void handleSumNode(Node *node);
  void checkEasySimplify(Node* node);

public:
  void printTree();
  void printDif();
  void simplify();
  void differentiate();

};

#endif
