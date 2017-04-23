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

bool isNumeric(char a){
  return ( (a >= '0') && (a <= '9') ) || (a == '.');
}

bool isLetter(char a){
  return (a >= 'a') && (a <= 'z');
}

bool isSimpleOP(char a){
  return (a == '+') ||
         (a == '-') ||
         (a == '*') ||
         (a == '/') ||
         (a == '^');
}

bool isOpenBracket(char a){
  return (a == '(');
}

bool isCloseBracket(char a){
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

  else
    return TOKEN_TYPE::VAR;
}

bool is_unary_minus(Parser::MODE last_mode){
  return (last_mode != Parser::MODE::NUMERIC)      &&
         (last_mode != Parser::MODE::LETTER) &&
         (last_mode != Parser::MODE::CLOSE_BRACKET);
}

Parser::MODE Parser::getMode(char a, Parser::MODE last_mode){
  return
    (a == '-') && ( is_unary_minus(last_mode) )  ? Parser::MODE::NUMERIC       :
    isNumeric(a)                                 ? Parser::MODE::NUMERIC       :
    isLetter(a)                                  ? Parser::MODE::LETTER        :
    isSimpleOP(a)                                ? Parser::MODE::BINARY_OPERATOR      :
    isOpenBracket(a)                             ? Parser::MODE::OPEN_BRACKET  :
    isCloseBracket(a)                            ? Parser::MODE::CLOSE_BRACKET :
    Parser::MODE::ERROR;
}

bool Parser::get_next_token(string *input, size_t &str_position,Parser::MODE &mode, Token &token){
  Parser::MODE last_mode = mode;
  mode = getMode(input->at(str_position), last_mode);

  if (mode == Parser::MODE::ERROR){
    cout << "Parser error after: \n" << input->substr(0, str_position) << '\n';
    return false;
  }

  size_t offset;
  token = Token();
  if (mode == Parser::MODE::BINARY_OPERATOR || mode == Parser::MODE::UNARY_OPERATOR || mode == Parser::MODE::OPEN_BRACKET ||  mode == Parser::MODE::CLOSE_BRACKET){
    token.value  = input->substr(str_position, 1);
    token.type = mode == Parser::MODE::BINARY_OPERATOR ? TOKEN_TYPE::BINARY_OP :
                 mode == Parser::MODE::UNARY_OPERATOR  ? TOKEN_TYPE::UNARY_OP :
                 TOKEN_TYPE::BRA;
    ++str_position;
  }

  if (mode == Parser::MODE::NUMERIC || mode == Parser::MODE::LETTER){
    bool (*f)(char a) = mode == Parser::MODE::NUMERIC ? isNumeric : isLetter;
    offset = 1;
    while (str_position + offset < input->size() && f(input->at(str_position + offset)))
      ++offset;

    token.value = input->substr(str_position, offset);
    if (mode == Parser::MODE::NUMERIC && (std::count(token.value.begin(), token.value.end(), '.') > 1)){
      mode = Parser::MODE::ERROR;
      cout << "Too many decimal points ('.')" << '\n';
      return false;
    }
    str_position += offset;

    token.type = mode == Parser::MODE::NUMERIC ? TOKEN_TYPE::NUM : getTokenType(&(token.value));
  }
  return true;
}

queue<Token> *Parser::parse_input(string *input){
  // Remove all spaces
  input->erase(remove_if(input->begin(), input->end(), [](char a){return std::isspace(a);}), input->end());

  vector<Token> tokens;
  Token token;
  size_t str_position = 0;
  MODE mode = Parser::MODE::START;

  while (str_position < input->size()){

    if (get_next_token(input, str_position, mode, token)){
      token.printToken();
      tokens.push_back(token);
    }
    else
      return nullptr;

  }
  cout << "\n\n";
  return to_queue(tokens);
}

queue<Token> *Parser::to_queue(vector<Token> &token_list){
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
      {"exp", OpInfo(5, "R")}
    };

    // //////// SHUNTING YARD /////////
  queue<Token> in_queue;
  queue<Token> *out_queue = new queue<Token>;
  stack<Token> op_stack;
  OpInfo op1, op2;
  for (Token tk : token_list){

    switch (tk.type) {
      case TOKEN_TYPE::NUM:
        out_queue->push(tk);
        break;
      case TOKEN_TYPE::VAR:
        out_queue->push(tk);
        break;
      case TOKEN_TYPE::UNARY_OP:
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

        break;
      case TOKEN_TYPE::BINARY_OP:
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

        break;

      case TOKEN_TYPE::BRA:
        if (tk.value == string("("))
          op_stack.push(tk);
        else {
          while (op_stack.top().value != string("(")){
            out_queue->push(op_stack.top());
            op_stack.pop();
            if (op_stack.empty()){
              cout << "Mismatched parenthesis \n";
              break;
            }
          }
          if (op_stack.top().value == string("(")){
            op_stack.pop();
          }
        }
        break;
      case TOKEN_TYPE::NONE:
        break;
    }
  }
  while (not op_stack.empty()){
    if (op_stack.top().type == TOKEN_TYPE::BRA){
      cout << "Mismatched parenthesis \n";
      break;
    }
    out_queue->push(op_stack.top());
    op_stack.pop();
  }

  return out_queue;
}
