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


namespace node_util {
  bool inline isZero(Node const *node){ return (node->token.type == TOKEN_TYPE::NUM && node->token.value == std::string("0"));  }
  bool inline isOne(Node const *node){ return (node->token.type == TOKEN_TYPE::NUM && node->token.value == std::string("1"));  }
  bool inline isVar(Node const *node){ return node->token.type == TOKEN_TYPE::VAR;  }
  bool inline isNum(Node const *node){ return node->token.type == TOKEN_TYPE::NUM;  }
  bool inline isTerminalNum(Node const *node){ return (not (node->left || node->right) && node->token.type == TOKEN_TYPE::NUM);  }
  double inline getNum(Node const *node){ return std::stod(node->token.value);  }
}
#endif
