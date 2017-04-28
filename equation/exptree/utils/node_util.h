#ifndef NODEUTIL__H
#define NODEUTIL__H

#include "../exptree.h"


namespace node_util {
  typedef ExpTree::Node Node;

  Node* copyNode(Node *node);
  void copyChild(Node *node, Node *child_to_copy, Node *other);

  Node *newNode(Token const &token, Node *right, Node *left = nullptr);
  bool isZero(Node *node);
  bool isOne(Node *node);
  bool isVar(Node *node);
  bool isNum(Node *node);
  bool isTerminalNum(Node *node);
  double getNum(Node *node);
}

#endif
