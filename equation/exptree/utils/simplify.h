#ifndef SIMPLIFY__H
#define SIMPLIFY__H

#include "../exptree.h"

namespace simplify {

  typedef ExpTree::Node Node;
  void simplifyNode(Node *node);
  void simplifyBinaryOp(Node *node);
  void simplifyPlus(Node *node);
  void simplifyMul(Node *node);
  void simplifyPow(Node *node);
}

#endif
