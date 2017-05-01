#ifndef NODEUTIL__H
#define NODEUTIL__H

#include "../exptree.h"


namespace node_util {
  typedef ExpTree::Node Node;

  std::unique_ptr<Node> cpyNode(Node const *node);

  std::unique_ptr<Node> newNode(Token const &token, std::unique_ptr<Node> &right, std::unique_ptr<Node> &left);
  std::unique_ptr<Node> newNode(Token const &token, std::unique_ptr<Node> &&right, std::unique_ptr<Node> &&left);
  std::unique_ptr<Node> newNode(Token const &token, std::unique_ptr<Node> &right);
  std::unique_ptr<Node> newNode(Token const &token, std::unique_ptr<Node> &&right);
  bool isZero(Node const *node);
  bool isOne(Node const *node);
  bool isVar(Node const *node);
  bool isNum(Node const *node);
  bool isTerminalNum(Node const *node);
  double getNum(Node const *node);
  std::string doubleToString(double a);
}

#endif
