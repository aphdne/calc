#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#include <cctype>

#define PRINTC(x) std::cout << #x": " << x << ",\t";
#define PRINTN(x) std::cout << #x": " << x << "\n";
#define PRINT_TOKENS(x) for (Token& t : x)\
                       std::cout << t.lexeme << ", " << t.type << "\n"

// TODO: floating numbers, variables, encapsulation?

struct Token {
  enum Type {
    Undefined = 0,
    Integer,
    OpenParen,
    CloseParen,
    Divide,
    Multiply,
    Plus,
    Minus,
  };

  std::string lexeme;
  Type type;
};

inline void error(std::string_view msg);
static int get_operation_order(Token::Type type);
static std::string int_to_str(int num);
inline bool is_operator(const Token& token);
static std::ostream& operator<<(std::ostream& out, const Token::Type type);
static void operate(std::vector<Token>& tokens, int op_i);
static void parse(std::vector<Token>& tokens);
static int str_to_int(std::string str);
static void tokenise(std::vector<Token>& tokens, std::string_view statement);

void error(std::string_view msg) {
  std::cout << "\e[1;31m[error] " << msg << "\e[0m\n";
}

int get_operation_order(Token::Type type) {
  switch (type) {
    case Token::Type::Divide:   return 0;
    case Token::Type::Multiply: return 0;
    case Token::Type::Plus:     return 1;
    case Token::Type::Minus:    return 1;
  }
}

std::string int_to_str(int num) {
  if (num == 0) return "0";

  bool negative{};

  if (num < 0) {
    negative = true;
    num -= num*2;
  }

  int super{};
  while (std::pow(10, super) <= num)
    super++;

  std::string r{};
  for (int i = super; i > 0; i--) {
    char c = (num / static_cast<int>(std::pow(10, i-1))) % 10 + 48;
    r += c;
  }

  if (negative)
    r.insert(r.begin(), '-');

  return r;
}

bool is_operator(const Token& token) {
  return token.type >= Token::Type::Divide;
}

std::ostream& operator<<(std::ostream& out, const Token::Type type) {
  switch (type) {
    case Token::Type::Integer:     return out << "DIGIT";
    case Token::Type::Plus:      return out << "PLUS";
    case Token::Type::Minus:     return out << "MINUS";
    case Token::Type::Divide:    return out << "DIVIDE";
    case Token::Type::Multiply:  return out << "MULTIPLY";
    default:                     return out << "UNDEFINED";
  }
}

void operate(std::vector<Token>& tokens, int op_i) {
  Token& token = tokens[op_i];

  int lhs_i = op_i - 1;
  int rhs_i = op_i + 1;

  for (; lhs_i > 0, tokens[lhs_i].type != Token::Type::Integer; lhs_i--) {
    if (tokens[lhs_i].type != Token::Type::Undefined)
      error("operators require a digit on their left side");
  }

  for (; rhs_i > 0, tokens[rhs_i].type != Token::Type::Integer; rhs_i++) {
    if (tokens[rhs_i].type != Token::Type::Undefined)
      error("operators require a digit on their right side");
  }

  int lhs = str_to_int(tokens[lhs_i].lexeme);
  int rhs = str_to_int(tokens[rhs_i].lexeme);

  int r{};
  switch (token.type) {
  case Token::Type::Divide:   r = lhs / rhs; break;
  case Token::Type::Multiply: r = lhs * rhs; break;
  case Token::Type::Plus:     r = lhs + rhs; break;
  case Token::Type::Minus:    r = lhs - rhs; break;
  }

  token = {int_to_str(r), Token::Type::Integer};

  tokens[lhs_i] = {"", Token::Type::Undefined};
  tokens[rhs_i] = {"", Token::Type::Undefined};
}

void parse(std::vector<Token>& tokens) {
  std::vector<int> operators{};
  for (int i = 0; i < tokens.size(); i++) {
    if (is_operator(tokens[i])) {
      if ((i == 0 || i >= tokens.size()-1) || (is_operator(tokens[i-1]) || is_operator(tokens[i+1]))) {
        error("invalid operator usage");
        return;
      }
      operators.push_back(i);
    }
  }

  // sort by order of operation (defined in Token enum)
  std::sort(operators.begin(), operators.end(), [tokens](int a, int b) {
                                                  return get_operation_order(tokens[a].type) < get_operation_order(tokens[b].type);
                                                });

  std::vector<std::pair<int,int>> parentheses{};
  int close_amt{};
  for (int i = 0; i < tokens.size(); i++) {
    if (tokens[i].type == Token::Type::OpenParen) {
      int open_amt{1};
      for (int j = i+1; j < tokens.size(); j++) {
        if (tokens[j].type == Token::Type::OpenParen) {
          open_amt++;
        } else if (tokens[j].type == Token::Type::CloseParen) {
          close_amt--;
          open_amt--;
          if (open_amt == 0) {
            parentheses.push_back({i, j});
            break;
          }
        }
      }
      if (open_amt > 0) {
        error("unmatched open parenthesis '('");
        return;
      }
    } else if (tokens[i].type == Token::Type::CloseParen) {
      close_amt++;
    }
  }
  if (close_amt > 0) {
    error("unmatched close parenthesis ')'");
    return;
  }

  for (const std::pair<int, int>& p : parentheses) {
    for (int i = 0; i < operators.size(); i++) {
      int o = operators[i];
      if (o > p.first && o < p.second) {
        operate(tokens, o);
        operators.erase(operators.begin() + i);
      }
      tokens[p.first] = {"", Token::Type::Undefined};
      tokens[p.second] = {"", Token::Type::Undefined};
    }
  }

  for (int i = 0; i < operators.size(); i++) {
    operate(tokens, operators[i]);
  }
}

int str_to_int(std::string str) {
  int result{};
  bool negative{};

  if (str[0] == '+' || str[0] == '-') {
    if (str[0] == '-') {
      negative = true;
    }
    str = str.substr(1, str.size()-1);
  }

  for (int i = 0; i < str.size(); i++) {
    int i_rev = str.size()-1-i;
    int ch    = static_cast<int>(str[i_rev]) - 48; // digit chars begin at 48
    result += ch * std::pow(10, i);
  }

  if (negative)
    result -= result*2;

  return result;
}

void tokenise(std::vector<Token>& tokens, std::string_view statement) {
  std::string lexeme{};
  Token::Type prev_type{};

  for (int i = 0; i < statement.size(); i++) {
    char ch = statement.at(i);

    Token::Type curr_type{};

    if (std::isdigit(ch)) {
      curr_type = Token::Type::Integer;
    } else {
      switch (ch) {
      case '(': curr_type = Token::Type::OpenParen;  break;
      case ')': curr_type = Token::Type::CloseParen; break;
      case '*': curr_type = Token::Type::Multiply;   break;
      case '/': curr_type = Token::Type::Divide;     break;
      case '+': curr_type = Token::Type::Plus;
        if (i < statement.size() - 1 && !(prev_type == Token::Type::Integer || (prev_type == Token::Type::Undefined && i != 0)) && std::isdigit(statement.at(i+1)))
          curr_type = Token::Type::Integer;
        break;
      case '-': curr_type = Token::Type::Minus;
        if (i < statement.size() - 1 && !(prev_type == Token::Type::Integer || (prev_type == Token::Type::Undefined && i != 0)) && std::isdigit(statement.at(i+1)))
          curr_type = Token::Type::Integer;
        break;
      }
    }

    if (i == 0)
      prev_type = curr_type;

    if (curr_type != prev_type && prev_type != Token::Type::Undefined) {
      tokens.push_back({lexeme, prev_type});
      lexeme = "";
    }

    if (curr_type != Token::Type::Undefined)
      lexeme += ch;

    prev_type = curr_type;
  }
}

int main(int argc, char* argv[]) {
  std::string statement{};
  std::vector<Token> tokens{};

  if (argc == 1) {
    std::cout << "github: aphdne/calc\n";
    std::cout << "> ";

    while (getline(std::cin, statement)) {
      statement += ' ';

      tokenise(tokens, statement);
      parse(tokens);

      for (Token& t : tokens)
        if (t.type != Token::Type::Undefined)
          std::cout << t.lexeme << "\n";

      tokens.clear();
      std::cout << "> ";
    }
  } else {
    for (int i = 1; i < argc; i++) {
      std::string s = argv[i];
      statement += s;
    }
    statement += ' ';

    tokenise(tokens, statement);
    parse(tokens);

    for (Token& t : tokens)
      if (t.type != Token::Type::Undefined)
        std::cout << t.lexeme << "\n";
  }
}
