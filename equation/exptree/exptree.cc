#include "exptree.h"

using namespace std;

ExpTree::ExpTree(){
  exp_tree = new Node();
  exp_tree->token = Token(TOKEN_TYPE::BINARY_OP, string("+"));
  exp_tree->left = new Node();
  exp_tree->left->token = Token(TOKEN_TYPE::NUM, "3");
  exp_tree->right = new Node();
  exp_tree->right->token = Token(TOKEN_TYPE::NUM, "3");
}

ExpTree::ExpTree(std::queue<Token> *token_queue){

  Node *node = new Node;


  stack<Token> *stck = new stack<Token>;
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
        stck->push(current);
        break;
      case TOKEN_TYPE::BINARY_OP:
        node->left = new Node(stck->top());
        stck->pop();
        node->right = new Node(stck->top());
        stck->pop();
        node->token = current;
        break;
      // case TOKEN_TYPE::UNARY_OP:
        // if (stck->size() < 1){
        //   syntax_error = true;
        //   break;
        // }
        // a = stck->top();
        // stck->pop();
        // stck->push(un_maps[current.value](a));
        // break;
      // default:
      //   break;
    }
  }
  printTreeInternal(node);
}


void ExpTree::printTree(){
  printTreeInternal(exp_tree);
}

void ExpTree::printTreeInternal(Node *node){
  if (node->left){
    cout << "(";
    printTreeInternal(node->left);
  }
  node->token.printVal();
  if (node->right){
    printTreeInternal(node->right);
    cout << ")";
  }
}
