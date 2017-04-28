#ifndef DIFF__H
#define DIFF__H

#include "exptree.h"
#include "node_util.h"

namespace diff {

  typedef ExpTree::Node Node;

  Node *newNode(Token const &token, Node *right, Node *left = nullptr);
  Node *differentiateNode(Node *node);

  Node *diffPlus(Node *node);
  Node *diffMin(Node *node);
  Node *diffMul(Node *node);
  Node *diffPow(Node *node);
  Node *diffDiv(Node *node);

  Node *diffUnaryMinus(Node *node);
  Node *diffSin(Node *node);
  Node *diffExp(Node *node);
  Node *diffCos(Node *node);
  Node *diffLog(Node *node);

  Node *numNode(std::string &&  str);

};


#endif
