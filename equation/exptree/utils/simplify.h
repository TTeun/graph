#ifndef SIMPLIFY__H
#define SIMPLIFY__H

#include "../exptree.h"
#include <memory>

namespace simplify {

  typedef ExpTree::Node Node;
  void simplifyNode(std::unique_ptr<Node> &node);
  void simplifyBinaryOp(std::unique_ptr<Node> &node);
  void simplifyPlus(std::unique_ptr<Node> &node);
  void simplifyMul(std::unique_ptr<Node> &node);
  void simplifyPow(std::unique_ptr<Node> &node);
}

#endif
