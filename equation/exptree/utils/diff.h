#ifndef DIFF__H
#define DIFF__H

#include "../exptree.h"
#include "node_util.h"

namespace diff {

  typedef ExpTree::Node Node;
  typedef std::unique_ptr<Node> u_node;
  u_node differentiateNode(Node const *node);

  u_node diffPlus(Node const *node);
  u_node diffMin(Node const *node);
  u_node diffMul(Node const *node);
  u_node diffPow(Node const *node);
  u_node diffDiv(Node const *node);

  u_node diffUnaryMinus(Node const *node);
  u_node diffSin(Node const *node);
  u_node diffExp(Node const *node);
  u_node diffCos(Node const *node);
  u_node diffLog(Node const *node);

  u_node numNode(std::string &&  str);
}


#endif
