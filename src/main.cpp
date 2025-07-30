#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

#define PRINTC(x) std::cout << #x": " << x << ",\t";
#define PRINTN(x) std::cout << #x": " << x << "\n";

inline void error(std::string_view msg) {
  std::cout << "\e[1;31m[error] " << msg << "\e[0m\n";
}

struct Token {
  enum Type {
    Undefined = 0,
    Digit,
    Divide,
    Multiply,
    Plus,
    Minus,
  };

  std::string lexeme;
  Type type;
};

inline bool is_operator(const Token& t) {
  return t.type >= Token::Type::Divide;
}

inline bool is_number(char ch) {
  return ch >= '0' && ch <= '9';
}

std::ostream& operator<<(std::ostream& out, const Token::Type ttype) {
  switch (ttype) {
    case Token::Type::Digit:     return out << "DIGIT";
    case Token::Type::Plus:      return out << "PLUS";
    case Token::Type::Minus:     return out << "MINUS";
    case Token::Type::Divide:    return out << "DIVIDE";
    case Token::Type::Multiply:  return out << "MULTIPLY";
    default:                     return out << "UNDEFINED";
  }
}

int str_to_int(std::string str) {
  int result{};
  bool negative{};

  if (str[0] == '-') {
    negative = true;
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

std::string int_to_str(int p_int) {
  if (p_int == 0) return "0";

  bool negative{};

  if (p_int < 0) {
    negative = true;
    p_int -= p_int*2;
  }

  int super{};
  while (std::pow(10, super) <= p_int)
    super++;

  std::string r{};
  for (int i = super; i > 0; i--) {
    char c = (p_int / static_cast<int>(std::pow(10, i-1))) % 10 + 48;
    r += c;
  }

  if (negative)
    r.insert(r.begin(), '-');

  return r;
}

void tokenise(std::vector<Token>& tokens, std::string_view statement) {
  std::string lexeme{};
  Token::Type prev_type{};

  for (int i = 0; i < statement.size(); i++) {
    char ch = statement.at(i);

    Token::Type curr_type{};

    if (is_number(ch)) {
      curr_type = Token::Type::Digit;
    } else {
      switch (ch) {
      case '*': curr_type = Token::Type::Multiply; break;
      case '/': curr_type = Token::Type::Divide; break;
      case '+': curr_type = Token::Type::Plus; break;
      case '-': curr_type = Token::Type::Minus;
        if (i < statement.size() - 1 && !(prev_type == Token::Type::Digit || prev_type == Token::Type::Undefined) && is_number(statement.at(i+1)))
          curr_type = Token::Type::Digit;
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

  std::sort(operators.begin(), operators.end(), [tokens](int a, int b) {
                                                  return tokens[a].type < tokens[b].type;
                                                });

  for (int i = 0; i < operators.size(); i++) {
    int t_i = operators[i];
    Token& token = tokens[t_i];

    int lhs_i = t_i - 1;
    int rhs_i = t_i + 1;

    for (; lhs_i > 0, tokens[lhs_i].type != Token::Type::Digit; lhs_i--) {
      if (tokens[lhs_i].type != Token::Type::Undefined)
        error("operators require a digit on their left side");
    }

    for (; rhs_i > 0, tokens[rhs_i].type != Token::Type::Digit; rhs_i++) {
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

    token = {int_to_str(r), Token::Type::Digit};

    tokens[lhs_i] = {"", Token::Type::Undefined};
    tokens[rhs_i] = {"", Token::Type::Undefined};
  }
}

int main() {
  std::cout << "github: aphdne/calc\n";
  std::cout << "> ";

  std::string statement{};
  std::vector<Token> tokens{};
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
}
