#ifndef EXPTREE__H
#define EXPTREE__H

// #include "../expression/expression.h"
#include <queue>
#include <memory>
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
    std::unique_ptr<Node> left = nullptr;
    std::unique_ptr<Node> right = nullptr;
  };
private:
  std::unique_ptr<Node> exp_tree = nullptr;
  std::unique_ptr<Node> dif_tree = nullptr;

  void clearNode(std::unique_ptr<Node> &node);
  void printTreeInternal(std::unique_ptr<Node> &node);

public:
  void setTree(std::queue<Token> *token_queue);
  void printTree();
  void printDif();
  void simplify();
  void differentiate();
  void clearTree();

};

#endif
