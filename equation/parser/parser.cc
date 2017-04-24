#include "parser.h"
#include <cctype> // isspace
#include <algorithm> // remove_if
#include <stack>

using namespace std;

struct OpInfo{
  OpInfo(){
    pres = 0;
    ass = 'L';
  };
  ~OpInfo(){}
  OpInfo(size_t p, char const * a){
    pres = p;
    ass = *a;
  }
  size_t pres;
  char ass;
};

inline bool isNumeric(char a){
  return  (a >= '0') && (a <= '9');
}

inline bool isLetter(char a){
  return (a >= 'a') && (a <= 'z');
}

// NUMERIC,
// LETTER,
// UNARY_MINUS,
// UNARY_OPERATOR,
// BINARY_OPERATOR,
// OPEN_BRACKET,
// CLOSE_BRACKET,
// ERROR,
// START

inline bool isSimpleOP(char a, Parser::MODE last_mode){
  return
        ( (last_mode != Parser::MODE::UNARY_MINUS)     &&
          (last_mode != Parser::MODE::UNARY_OPERATOR)  &&
          (last_mode != Parser::MODE::BINARY_OPERATOR) &&
          (last_mode != Parser::MODE::START)  )
    &&  ( (a == '+') ||
          (a == '-') || // This is a binary minus if we reach here
          (a == '*') ||
          (a == '/') ||
          (a == '^') );
}

inline bool isOpenBracket(char a){
  return (a == '(');
}

inline bool isCloseBracket(char a){
  return (a == ')');
}

TOKEN_TYPE getTokenType(string *str){
  if (
    *str == string("sin") ||
    *str == string("cos") ||
    *str == string("tan") ||
    *str == string("exp")
  )
    return TOKEN_TYPE::UNARY_OP;

  if (*str == string("pi"))
    return TOKEN_TYPE::NUM;

  else
    return TOKEN_TYPE::VAR;
}

inline bool is_unary_minus(char a, Parser::MODE last_mode){
  return (a == '-')                                  &&
         ( (last_mode == Parser::MODE::START)        ||
           (last_mode == Parser::MODE::CLOSE_BRACKET) );
}

Parser::MODE Parser::getMode(char a, Parser::MODE last_mode){
  return
    is_unary_minus(a, last_mode)  ? Parser::MODE::UNARY_MINUS  :
    isNumeric(a)                  ? Parser::MODE::NUMERIC         :
    isLetter(a)                   ? Parser::MODE::LETTER          :
    isSimpleOP(a, last_mode)      ? Parser::MODE::BINARY_OPERATOR :
    isOpenBracket(a)              ? Parser::MODE::OPEN_BRACKET    :
    isCloseBracket(a)             ? Parser::MODE::CLOSE_BRACKET   :
    Parser::MODE::ERROR;
}

bool readNum(
              string *input,
              size_t &str_position,
              Parser::MODE &mode,
              Token &token
            )
{
  size_t offset = 1; // This will keep track of how many characters the number consists of
  while (
        str_position + offset < input->size() &&
          ( isNumeric(input->at(str_position + offset))
            || input->at(str_position + offset) == '.'  )
      )
    ++offset;

  token.value = input->substr(str_position, offset);
  if (count(token.value.begin(), token.value.end(), '.') > 1)
    {
    mode = Parser::MODE::ERROR;
    cout << "Syntax error" << '\n';
    return false;
  }
  str_position += offset;
  token.type = TOKEN_TYPE::NUM;
  return true;
}

bool readWord(
                string *input,
                size_t &str_position,
                Parser::MODE &mode,
                Token &token
              )
{
  size_t offset = 1; // This will keep track of how many characters the number consists of
  while (
        str_position + offset < input->size() &&
        isLetter(input->at(str_position + offset))
      )
    ++offset;

  token.value = input->substr(str_position, offset);

  str_position += offset;
  token.type = getTokenType(&(token.value));
  return true;
}

bool singleLetterSymbol(
                          string *input,
                          size_t &str_position,
                          Parser::MODE &mode,
                          Token &token
                        )
{
  token.value  = input->substr(str_position, 1);
  token.type = mode == Parser::MODE::BINARY_OPERATOR ? TOKEN_TYPE::BINARY_OP :
                       TOKEN_TYPE::BRA;
  ++str_position;
  return true;
}

bool Parser::get_next_token(
                             string *input,
                             size_t &str_position,
                             Parser::MODE &mode,
                             Token &token
                          ){
  while (isspace((*input)[str_position])) // Skip first whitespace
    ++str_position;

  if (str_position == input->size())
  { // All we did was remove excess white space. This is not incorrect though!
    token.type = TOKEN_TYPE::NONE;
    return true;
  }

  Parser::MODE last_mode = mode;
  mode = getMode(input->at(str_position), last_mode); // See what we will now read
  token = Token();

  switch (mode)
  {
    case Parser::MODE::ERROR:
      return false;

    case Parser::MODE::UNARY_MINUS:
      token.type = TOKEN_TYPE::UNARY_OP;
      token.value = string("-u"); // Special notation for unary minus
      ++str_position;
      return true;

    case Parser::MODE::BINARY_OPERATOR:
      return singleLetterSymbol(input, str_position, mode, token);

    case Parser::MODE::OPEN_BRACKET:
      return singleLetterSymbol(input, str_position, mode, token);

    case Parser::MODE::CLOSE_BRACKET:
      return singleLetterSymbol(input, str_position, mode, token);

    case Parser::MODE::NUMERIC:
      return readNum(input, str_position, mode, token);

    case Parser::MODE::LETTER:
      return readWord(input, str_position, mode, token);

    default:
      return true;
  }

}

Expression *Parser::parse_input(string *input){

  vector<Token> tokens;
  Token token;
  size_t str_position = 0;
  MODE mode = Parser::MODE::START;
  bool parser_error = false;
  while (str_position < input->size()){

    if (get_next_token(input, str_position, mode, token))
    {
      if (token.type != TOKEN_TYPE::NONE) // A NONE token may happen with excess white space
        tokens.push_back(token);
    }
    else {
      parser_error = true;
      break;
    }
  }
  if (not parser_error)
    return to_queue(tokens);

  Expression *e = new Expression;
  e->setState( "Parser error after: \n" + input->substr(0, str_position) + '\n');
  return e;
}

void handleOp(queue<Token> *out_queue, stack<Token> &op_stack, Token &tk, unordered_map<string, OpInfo> &info){
  OpInfo op1, op2;

  op1 = info[tk.value];
  while (not op_stack.empty()){
    op2 = info[op_stack.top().value];

    if (  (op1.ass == 'L' && op1.pres <= op2.pres)
        ||(op1.ass == 'R' && op1.pres < op2.pres) ){
          out_queue->push(op_stack.top());
          op_stack.pop();
      }
    else
     break;
  }
  op_stack.push(tk);
}

bool handleBracket(queue<Token> *out_queue, stack<Token> &op_stack, Token &tk){
  if (tk.value == string("("))
    op_stack.push(tk);
  else {
    while (op_stack.top().value != string("(")){
      out_queue->push(op_stack.top());
      op_stack.pop();
      if (op_stack.empty()){
        return false; // Found Mismatched brackets
      }
    }
    if (op_stack.top().value == string("(")){
      op_stack.pop();
    }
  }
  return true;
}

bool emptyQueue(queue<Token> *out_queue, stack<Token> &op_stack){

  while (not op_stack.empty()){
    if (op_stack.top().type == TOKEN_TYPE::BRA)
      return false; // Found Mismatched brackets

    out_queue->push(op_stack.top());
    op_stack.pop();
  }
  return true;
}


Expression *Parser::to_queue(vector<Token> &token_list){
  static unordered_map<string, OpInfo> info =
    {
      {"+", OpInfo(2, "L")},
      {"-", OpInfo(2, "L")},
      {"*", OpInfo(3, "L")},
      {"/", OpInfo(3, "L")},
      {"^", OpInfo(4, "R")},
      {"sin", OpInfo(5, "R")},
      {"cos", OpInfo(5, "R")},
      {"tan", OpInfo(5, "R")},
      {"exp", OpInfo(5, "R")},
      {"-u", OpInfo(5, "R")}
    };

  ////////// SHUNTING YARD /////////
  queue<Token> in_queue;
  queue<Token> *out_queue = new queue<Token>;
  stack<Token> op_stack;
  vector<string> *variables = new vector<string>;

  bool paren_mismatch = false;

  OpInfo op1, op2;
  for (Token tk : token_list){
    switch (tk.type) {
      case TOKEN_TYPE::NUM:
        out_queue->push(tk);
        break;

      case TOKEN_TYPE::VAR:
        out_queue->push(tk);
        variables->push_back(tk.value);
        break;

      case TOKEN_TYPE::UNARY_OP:
        handleOp(out_queue, op_stack, tk, info);
        break;

      case TOKEN_TYPE::BINARY_OP:
        handleOp(out_queue, op_stack, tk, info);
        break;

      case TOKEN_TYPE::BRA:
        paren_mismatch = not handleBracket(out_queue, op_stack, tk);
        break;
      case TOKEN_TYPE::NONE:
        break;
    }

    if (paren_mismatch)
      break;
  }

  paren_mismatch = not emptyQueue(out_queue, op_stack);

  Expression *e = new Expression(out_queue, variables);
  if (paren_mismatch)
    e->setState(string("Mismatched parenthesis\n"));

  return e;
}
