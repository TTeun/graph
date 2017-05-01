#include "node_util.h"
#include <string>
#include <sstream>

using namespace std;

namespace node_util{

  u_node newNode(Token const &token, u_node &right, u_node &left){
    auto new_node = make_unique<Node>(token);
    new_node->left = move(left);
    new_node->right = move(right);
    return new_node;
  }

  u_node newNode(Token const &token, u_node &&right, u_node &&left){
    auto new_node = make_unique<Node>(token);
    new_node->left = move(left);
    new_node->right = move(right);
    return new_node;
  }

  u_node newNode(Token const &token, u_node &right){
    auto new_node = make_unique<Node>(token);
    new_node->right = move(right);
    return new_node;
  }

  u_node newNode(Token const &token, u_node &&right){
    auto new_node = make_unique<Node>(token);
    new_node->right = move(right);
    return new_node;
  }

  u_node cpyNode(Node const *node){
    auto new_node(make_unique<Node>(node->token));
    if (node->left)
      new_node->left = cpyNode(node->left.get());

    if (node->right)
      new_node->right = cpyNode(node->right.get());

    return new_node;
  }


  string doubleToString(double a){
    ostringstream ostr;
    ostr << a;
    return ostr.str();
  }

}
