#ifndef NODEUTIL__H
#define NODEUTIL__H

#include "exptree.h"

namespace node_util {
  typedef ExpTree::Node Node;

  Node* copyNode(Node *node);
  void copyRight(Node *node);
  void copyLeft(Node *node);
  bool isNum(Node *node);
  double getNum(Node *node);
  Node *newNode(Token const &token, Node *right, Node *left = nullptr);

}

#endif
