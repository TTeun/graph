#ifndef DIFF__H
#define DIFF__H

#include "../exptree.h"
#include "node_util.h"

namespace diff {

  typedef ExpTree::Node Node;

  std::unique_ptr<Node> differentiateNode(std::unique_ptr<Node> &node);

  std::unique_ptr<Node> diffPlus(std::unique_ptr<Node> &node);
  std::unique_ptr<Node> diffMin(std::unique_ptr<Node> &node);
  std::unique_ptr<Node> diffMul(std::unique_ptr<Node> &node);
  std::unique_ptr<Node> diffPow(std::unique_ptr<Node> &node);
  std::unique_ptr<Node> diffDiv(std::unique_ptr<Node> &node);

  std::unique_ptr<Node> diffUnaryMinus(std::unique_ptr<Node> &node);
  std::unique_ptr<Node> diffSin(std::unique_ptr<Node> &node);
  std::unique_ptr<Node> diffExp(std::unique_ptr<Node> &node);
  std::unique_ptr<Node> diffCos(std::unique_ptr<Node> &node);
  std::unique_ptr<Node> diffLog(std::unique_ptr<Node> &node);

  std::unique_ptr<Node> numNode(std::string &&  str);

};


#endif
