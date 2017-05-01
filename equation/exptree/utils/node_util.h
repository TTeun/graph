#ifndef NODEUTIL__H
#define NODEUTIL__H

#include "../exptree.h"


namespace node_util {
  typedef ExpTree::Node Node;
  typedef std::unique_ptr<Node> u_node;
  u_node cpyNode(Node const *node);

  u_node newNode(Token const &token, u_node &right, u_node &left);
  u_node newNode(Token const &token, u_node &&right, u_node &&left);
  u_node newNode(Token const &token, u_node &right);
  u_node newNode(Token const &token, u_node &&right);
  bool isZero(Node const *node);
  bool isOne(Node const *node);
  bool isVar(Node const *node);
  bool isNum(Node const *node);
  bool isTerminalNum(Node const *node);
  double getNum(Node const *node);
  std::string doubleToString(double a);
}

#endif
