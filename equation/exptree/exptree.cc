#include "exptree.h"
#include <map>
#include <iostream>
#include <stack>
#include "../eqmaps/eqmaps.h"
#include "utils/diff.h"
#include "utils/node_util.h"
#include "utils/simplify.h"

using namespace std;
using namespace node_util;

ExpTree::ExpTree(){}

ExpTree::ExpTree(std::queue<Token> *token_queue)
{

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
  clearNode(dif_tree);
}

void ExpTree::simplify(){
  simplify::simplifyNode(exp_tree);
}

void ExpTree::differentiate(){
  dif_tree = diff::differentiateNode(exp_tree);
  simplify::simplifyNode(dif_tree);
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
