#include "exptree.h"
#include <sstream>

using namespace std;

ExpTree::ExpTree(){
  cout << "ExpTree empty constructor\n";
  exp_tree = new Node();
  exp_tree->token = Token(TOKEN_TYPE::BINARY_OP, string("+"));
  exp_tree->left = new Node();
  exp_tree->left->token = Token(TOKEN_TYPE::NUM, "3");
  exp_tree->right = new Node();
  exp_tree->right->token = Token(TOKEN_TYPE::NUM, "3");
  Node *left  = exp_tree;
  exp_tree = new Node();
  exp_tree->token = Token(TOKEN_TYPE::BINARY_OP, string("+"));
  exp_tree->left = new Node();
  exp_tree->left->token = Token(TOKEN_TYPE::NUM, "3");
  exp_tree->right = new Node();
  exp_tree->right->token = Token(TOKEN_TYPE::NUM, "3");
  Node *right  = exp_tree;
  exp_tree = new Node();
  exp_tree->token = Token(TOKEN_TYPE::BINARY_OP, string("+"));
  exp_tree->left = left;
  exp_tree->right = right;
}

ExpTree::ExpTree(std::queue<Token> *token_queue){

  Node *node = new Node;
  stack<Node*> *n_stack = new stack<Node*>;
  Token current;

  while (not token_queue->empty())
  {
    current = token_queue->front();
    token_queue->pop();
    switch (current.type) {
      case TOKEN_TYPE::BRA:
        // paren_mismatch = true;
        break;
      case TOKEN_TYPE::NUM:
        node->token = current;
        n_stack->push(node);
        node = new Node;
        break;
      case TOKEN_TYPE::VAR:
        node->token = current;
        n_stack->push(node);
        node = new Node;
        break;
      case TOKEN_TYPE::BINARY_OP:
        node->right = n_stack->top();
        n_stack->pop();
        node->left = n_stack->top();
        n_stack->pop();
        node->token = current;
        n_stack->push(node);
        node = new Node;
        break;
      case TOKEN_TYPE::UNARY_OP:
        node->right = n_stack->top();
        n_stack->pop();
        node->token = current;
        n_stack->push(node);
        node = new Node;
        break;
      default:
        break;
    }
  }
  delete node; // We do not need a new node anymore
  exp_tree = n_stack->top();
  delete n_stack;
  simplify();
}

ExpTree::~ExpTree(){
  clearNode(exp_tree);
}

void ExpTree::simplify(){
  simplifyNode(exp_tree);
}

bool ExpTree::isZero(Node *node){
  return (node->token.type == TOKEN_TYPE::NUM && node->token.value == string("0"));
}

bool ExpTree::isOne(Node *node){
  return (node->token.type == TOKEN_TYPE::NUM && node->token.value == string("1"));
}

bool ExpTree::isVar(Node *node){
  return node->token.type == TOKEN_TYPE::VAR;
}

void ExpTree::cleanChildren(Node *node){
  if (node->left)
    delete node->left;
  if (node->right)
    delete node->right;
  node->left = nullptr;
  node->right = nullptr;
}

void ExpTree::checkEasySimplify(Node *node){
  if (node->token.value == string("+"))
    if (isZero(node->left) || isZero(node->right)){
      if (isZero(node->left))
        node->token = node->right->token;
      else
        node->token = node->left->token;

      cleanChildren(node);
      return;
    }

  if (node->token.value == string("*")){
    if (isZero(node->left) || isZero(node->right)){
      node->token = Token(TOKEN_TYPE::NUM, string("0"));
      cleanChildren(node);
      return;
    }

    if (isOne(node->left)){
      node->token = node->right->token;
      return;
      cleanChildren(node);
    }

    if (isOne(node->right)){
      node->token = node->left->token;
      cleanChildren(node);
      return;
    }
  }

  if (isVar(node->left) && isVar(node->right)){
    if (node->left->token.value != node->right->token.value)
      return;

    if (node->token.value == string("+")){
      node->token.value = string("*");
      node->left->token = Token(TOKEN_TYPE::NUM, string("2"));
    }
    if (node->token.value == string("-")){
      node->token  = Token(TOKEN_TYPE::NUM, string("0"));
      cleanChildren(node);
    }
  }

}

void ExpTree::simplifyNode(Node *node){
  if (node->token.type == TOKEN_TYPE::NUM)
    return;
  if (node->token.type == TOKEN_TYPE::VAR)
    return;

  if (node->left)
    simplifyNode(node->left);
  if (node->right)
    simplifyNode(node->right);

  if (node->token.type == TOKEN_TYPE::BINARY_OP){
    if (isTerminalNum(node->left) && isTerminalNum(node->right)){
      node->token.type = TOKEN_TYPE::NUM;
      ostringstream strs;
      strs << (EqMaps::bin_maps[node->token.value](getNum(node->left), getNum(node->right)));
      node->token.value = strs.str();
      delete node->left;
      delete node->right;
      node->left = nullptr;
      node->right = nullptr;
    }
    else
      checkEasySimplify(node);
  }

  if (node->token.type == TOKEN_TYPE::UNARY_OP)
    if (isTerminalNum(node->right)){
      node->token.type = TOKEN_TYPE::NUM;
      ostringstream strs;
      strs << (EqMaps::un_maps[node->token.value](getNum(node->right)));
      node->token.value = strs.str();
      delete node->right;
      node->right = nullptr;
    }
}

bool ExpTree::isTerminalNum(Node *node){
  return (not (node->left || node->right) && node->token.type == TOKEN_TYPE::NUM);
}

double ExpTree::getNum(Node *node){
  return stod(node->token.value);
}

ExpTree::Node *ExpTree::copyNode(Node *node){
  Node *new_node = new Node;
  if (node->left)
    new_node->left = copyNode(node->left);

  if (node->right)
    new_node->right = copyNode(node->right);

  new_node->token = node->token;
  return new_node;
}

void ExpTree::differentiate(){
  dif_tree = differentiateNode(exp_tree);
  simplifyNode(dif_tree);
}

ExpTree::Node *ExpTree::differentiateNode(Node *node){
  Node *new_node = new Node;
  if (node->token.type == TOKEN_TYPE::NUM){
    new_node->token = Token(TOKEN_TYPE::NUM, string("0"));
    return new_node;
  }
  if (node->token.type == TOKEN_TYPE::VAR){
    new_node->token = Token(TOKEN_TYPE::NUM, string("1"));
    return new_node;
  }
  if (node->token.type == TOKEN_TYPE::BINARY_OP){
    if (node->token.value == string("+") || node->token.value == string("-")){
      new_node->token.type = TOKEN_TYPE::BINARY_OP;
      new_node->token.value = node->token.value;
      new_node->left = differentiateNode(node->left);
      new_node->right = differentiateNode(node->right);
      return new_node;
    }
    if (node->token.value == string("*")){
      Node *left = new Node;
      Node *right = new Node;
      left->token = Token(TOKEN_TYPE::BINARY_OP, string("*"));
      left->left = differentiateNode(node->left);
      left->right = copyNode(node->right);

      right->token = Token(TOKEN_TYPE::BINARY_OP, string("*"));
      right->left = copyNode(node->left);
      right->right = differentiateNode(node->right);

      new_node->token = Token(TOKEN_TYPE::BINARY_OP, string("+"));
      new_node->left = left;
      new_node->right = right;
      return new_node;
    }

  }

  delete new_node;
  return nullptr;
}

void ExpTree::clearNode(Node *node){
  if (node->left)
    clearNode(node->left);
  if (node->right)
    clearNode(node->right);
  delete node;
}

void ExpTree::printTree(){
  printTreeInternal(exp_tree);
  cout << '\n';
}

void ExpTree::printDif(){
  printTreeInternal(dif_tree);
  cout << '\n';
}

void ExpTree::printTreeInternal(Node *node){
  cout << "(";
  if (node->left){
    printTreeInternal(node->left);
  }
  node->token.printVal();
  if (node->right){
    printTreeInternal(node->right);
  }
  cout << ")";
}
