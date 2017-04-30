#ifndef NODEUTIL__H
#define NODEUTIL__H

#include "../exptree.h"


namespace node_util {
  typedef ExpTree::Node Node;

  Node* copyNode(std::unique_ptr<Node> &node);

  std::unique_ptr<Node> &ewNode(Token const &token, std::unique_ptr<Node> &right, std::unique_ptr<Node> &left);
  bool isZero(std::unique_ptr<Node> &node);
  bool isOne(std::unique_ptr<Node> &node);
  bool isVar(std::unique_ptr<Node> &node);
  bool isNum(std::unique_ptr<Node> &node);
  bool isTerminalNum(std::unique_ptr<Node> &node);
  double getNum(std::unique_ptr<Node> &node);
  std::string doubleToString(double a);
}

#endif
