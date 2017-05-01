#ifndef DIFF__H
#define DIFF__H

#include "../exptree.h"
#include "node_util.h"

namespace diff {

  typedef ExpTree::Node Node;

  std::unique_ptr<Node> differentiateNode(Node const *node);

  std::unique_ptr<Node> diffPlus(Node const *node);
  std::unique_ptr<Node> diffMin(Node const *node);
  std::unique_ptr<Node> diffMul(Node const *node);
  std::unique_ptr<Node> diffPow(Node const *node);
  std::unique_ptr<Node> diffDiv(Node const *node);

  std::unique_ptr<Node> diffUnaryMinus(Node const *node);
  std::unique_ptr<Node> diffSin(Node const *node);
  std::unique_ptr<Node> diffExp(Node const *node);
  std::unique_ptr<Node> diffCos(Node const *node);
  std::unique_ptr<Node> diffLog(Node const *node);

  std::unique_ptr<Node> numNode(std::string &&  str);

}


#endif
