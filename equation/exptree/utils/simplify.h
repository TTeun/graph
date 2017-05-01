#ifndef SIMPLIFY__H
#define SIMPLIFY__H

#include "../exptree.h"
#include <memory>

namespace simplify {

  typedef ExpTree::Node Node;
  typedef std::unique_ptr<Node> u_node;
  void simplifyNode(u_node &node);
  void simplifyBinaryOp(u_node &node);
  void simplifyUnaryOp(u_node &node);
  void simplifyPlus(u_node &node);
  void simplifyMul(u_node &node);
  void simplifyPow(u_node &node);
  void simplifyDiv(u_node &node);
}

#endif
