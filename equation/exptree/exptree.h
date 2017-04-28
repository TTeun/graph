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

public:
  void printTree();
  void printDif();
  void simplify();
  void differentiate();

};

#endif
