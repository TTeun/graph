#include "exptree.h"

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
  // printTree();
}

ExpTree::ExpTree(std::queue<Token> *token_queue){

  Node *node = new Node;
  stack<Token> *stck = new stack<Token>;
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
  exp_tree = n_stack->top();
}


void ExpTree::printTree(){
  printTreeInternal(exp_tree);
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
