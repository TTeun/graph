#include "exptree.h"
#include <sstream>
#include <map>

using namespace std;

ExpTree::ExpTree(){}

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
  clearNode(dif_tree);
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

bool ExpTree::isNum(Node *node){
  return node->token.type == TOKEN_TYPE::NUM;
}

bool ExpTree::isVarExp(Node *node){
  return (
          node->token.value == string("^") &&
          node->left->token.type == TOKEN_TYPE::VAR &&
          node->right->token.type == TOKEN_TYPE::NUM
        );
}

void ExpTree::copyLeft(Node *node){
  node->token = node->left->token;
  Node *temp = node->left;
  delete node->right;
  node->right = node->left->right;
  node->left = node->left->left;
  delete temp;
}

void ExpTree::copyRight(Node *node){
  node->token = node->right->token;
  Node *temp = node->right;
  delete node->left;
  node->left = node->right->left;
  node->right = node->right->right;
  delete temp;
}
void ExpTree::cleanChildren(Node *node){
  node->left = nullptr;
  node->right = nullptr;
}

void ExpTree::checkEasySimplify(Node *node){
  if (node->token.value == string("+") || node->token.value == string("-")){
    if (isZero(node->left)){
      copyRight(node);
      return;
    }
    if (isZero(node->right)){
      copyLeft(node);
      return;
    }
  }

  if (node->token.value == string("*")){
    if (isZero(node->left) || isZero(node->right)){
      node->token = Token(TOKEN_TYPE::NUM, string("0"));
      clearNode(node->left);
      clearNode(node->right);
      cleanChildren(node);
      return;
    }

    if (isOne(node->left)){
      copyRight(node);
      return;
    }

    if (isOne(node->right)){
      copyLeft(node);
      return;
    }
  }

  if (isVar(node->left) && isVar(node->right)){
    if (node->left->token.value != node->right->token.value)
      return;

    if (node->token.value == string("+")){
      node->token.value = string("*");
      node->left->token = Token(TOKEN_TYPE::NUM, string("2"));
      return;
    }

    if (node->token.value == string("-")){
      node->token = Token(TOKEN_TYPE::NUM, string("0"));
      return;
      cleanChildren(node);
    }

    if (node->token.value == string("/")){
      node->token = Token(TOKEN_TYPE::NUM, string("1"));
      return;
      cleanChildren(node);
    }

    if (node->token.value == string("*")){
      node->token = Token(TOKEN_TYPE::BINARY_OP, string("^"));
      node->right->token = Token(TOKEN_TYPE::NUM, string("2"));
      return;
    }
  }

  if (node->token.value == string("^") && node->right->token.value == string("1")){
    copyLeft(node);
  }
}

bool ExpTree::isSumNode(Node *node){
  bool left = node->left ? isSumNode(node->left) : true;
  bool right = node->right ? isSumNode(node->right) : true;
  return ((node->token.value == string("+") || node->token.type == TOKEN_TYPE::NUM || node->token.type == TOKEN_TYPE::VAR) && left && right);
}

void ExpTree::getSumNodes(Node *node, vector<Node*> *sumNodes){
  if (node->token.type != TOKEN_TYPE::BINARY_OP)
    sumNodes->push_back(node);
  if (node->left)
    getSumNodes(node->left, sumNodes);
  if (node->right)
    getSumNodes(node->right, sumNodes);
}

void ExpTree::handleSumNode(Node *node){
  vector<Node*> *sumNodes = new vector<Node*>;
  getSumNodes(node, sumNodes);

  map<string, size_t> *var_counter = new map<string, size_t>;
  float sum = 0;
  for (Node *node : *sumNodes){
    switch (node->token.type) {
      case TOKEN_TYPE::NUM:
        sum += stod(node->token.value);
        break;
      case TOKEN_TYPE::VAR:
        {
          auto it = var_counter->find(node->token.value);
          if (it != var_counter->end())
            it->second++;
          else
            (*var_counter)[node->token.value] = 1;
        }
        break;
      case TOKEN_TYPE::BINARY_OP:
        break;

      case TOKEN_TYPE::UNARY_OP:
        break;

      case TOKEN_TYPE::NONE:
        break;
      default:
        break;
    }
  }
  delete sumNodes;
  clearNode(node->right);
  clearNode(node->left);
  ostringstream str1;

  if (var_counter->size() == 0){
    str1 << sum;
    node->token = Token(TOKEN_TYPE::NUM, str1.str());
    node->right = nullptr;
    node->left = nullptr;
    return;
  }
  vector<Node*> *var_nodes = new vector<Node*>;
  Node *new_node;
  for (auto &m : *var_counter){
    new_node = new Node;
    new_node->token = Token(TOKEN_TYPE::BINARY_OP, string("*"));
    new_node->right = new Node;
    new_node->right->token = Token(TOKEN_TYPE::VAR, m.first);
    new_node->left = new Node;
    str1.str(string(""));
    str1 << m.second;

    new_node->left->token = Token(TOKEN_TYPE::NUM, str1.str());
    var_nodes->push_back(new_node);
  }
  str1.str(string(""));
  str1 << sum;

  node->token = Token(TOKEN_TYPE::BINARY_OP, string("+"));
  node->left = new Node;
  node->left->token = Token(TOKEN_TYPE::NUM, str1.str());
  Node *parent = node;
  for (size_t idx = 0; idx < var_nodes->size() - 1; ++idx){
    parent->right = new Node;

    parent->right->token = Token(TOKEN_TYPE::BINARY_OP, string("+"));
    parent->right->left = var_nodes->at(idx);
    parent = parent->right;
  }
  parent->right = var_nodes->back();


  delete var_nodes;
  delete var_counter;
}

void ExpTree::simplifyNode(Node *node){
  if (node->token.type == TOKEN_TYPE::NUM)
    return;
  if (node->token.type == TOKEN_TYPE::VAR)
    return;

  if (isSumNode(node))
    handleSumNode(node);

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
    if (node->token.value == string("^")){
      if (isVar(node->left) && isNum(node->right)){
        ostringstream str1, str2;
        str1 << getNum(node->right) - 1;
        str2 << getNum(node->right);

        Node *left = new Node;
        Node *right = copyNode(node);
        right->right->token.value = str1.str();
        left->token = Token(TOKEN_TYPE::NUM, str2.str());

        new_node->token = Token(TOKEN_TYPE::BINARY_OP, string("*"));
        new_node->right = right;
        new_node->left = left;
        return new_node;
      }

    }

  }


  cout << "Shouldn't get here! Missing derivative pattern in exptree.cc\n";

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
