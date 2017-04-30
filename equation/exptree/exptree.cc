#include "exptree.h"
#include <map>
#include <iostream>
#include <stack>
#include "../eqmaps/eqmaps.h"
#include "utils/diff.h"
#include "utils/node_util.h"
#include "utils/simplify.h"

using namespace std;

ExpTree::ExpTree(){}

ExpTree::ExpTree(queue<Token> *token_queue)
{
  setTree(token_queue);
}

void ExpTree::setTree(queue<Token> *token_queue){
  Node *node = new Node;
  stack<Node*> *n_stack = new stack<Node*>;
  Token current;
  while (not token_queue->empty())
  {
    current = token_queue->front();
    token_queue->pop();
    switch (current.type) {
      case TOKEN_TYPE::BRA:
        cout << "paren_mismatch somehow?\n";
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
        node->right = unique_ptr<Node> (n_stack->top());
        n_stack->pop();
        node->left = unique_ptr<Node> (n_stack->top());
        n_stack->pop();
        node->token = current;
        n_stack->push(node);
        node = new Node;
        break;
      case TOKEN_TYPE::UNARY_OP:
        node->right = unique_ptr<Node> (n_stack->top());
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
  delete token_queue;
  exp_tree = unique_ptr<Node>(n_stack->top());
  delete n_stack;
  simplify();
}

ExpTree::~ExpTree(){
}

void ExpTree::simplify(){
  simplify::simplifyNode(exp_tree);
}

void ExpTree::differentiate(){
  dif_tree = diff::differentiateNode(exp_tree);
  // simplify::simplifyNode(dif_tree);
}

void ExpTree::printTree(){
  printTreeInternal(exp_tree);
  cout << '\n';
}

void ExpTree::printDif(){
  printTreeInternal(dif_tree);
  cout << '\n';
}

void ExpTree::printTreeInternal(std::unique_ptr<Node> &node){
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

void ExpTree::clearTree(){
  exp_tree = nullptr;
}
