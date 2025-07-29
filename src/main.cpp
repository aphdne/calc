#include <iostream>
#include <vector>
#include <cmath>

#define STATEMENT "3 - 616"
#define PRINTC(x) std::cout << #x": " << x << ",\t";
#define PRINTN(x) std::cout << #x": " << x << "\n";

inline void error(std::string_view msg) {
  std::cout << "\e[1;31m[error] " << msg << "\e[0m\n";
  std::exit(0);
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

int main() {
  std::string statement{STATEMENT};
  statement += ' ';
  std::vector<Token> tokens{};

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
      case '+': curr_type = Token::Type::Plus;
        if (i < statement.size() - 1 && prev_type != Token::Type::Digit && is_number(statement.at(i+1)))
          curr_type = Token::Type::Digit;
        break;
      case '-': curr_type = Token::Type::Minus;
        if (i < statement.size() - 1 && prev_type != Token::Type::Digit && is_number(statement.at(i+1)))
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

    // PRINTC(lexeme);
    // PRINTC(ch);
    // PRINTC(curr_type);
    // PRINTN(prev_type);

    prev_type = curr_type;
  }

  for (int i = 0; i < tokens.size(); i++) {
    Token& token = tokens[i];
    if (is_operator(token)) {
      if (i == 0 || i >= tokens.size()-1)
        continue;

      if (is_operator(tokens[i-1]) || is_operator(tokens[i+1])) {
        std::string out = tokens[i-1].lexeme + " " + tokens[i].lexeme + " " + tokens[i+1].lexeme;
        error("invalid operator usage: " + out);
      }

      int lhs = str_to_int(tokens[i-1].lexeme);
      int rhs = str_to_int(tokens[i+1].lexeme);

      int r{};
      switch (token.type) {
      case Token::Type::Divide:   r = lhs / rhs; break;
      case Token::Type::Multiply: r = lhs * rhs; break;
      case Token::Type::Plus:     r = lhs + rhs; break;
      case Token::Type::Minus:    r = lhs - rhs; break;
      }

      token = {int_to_str(r), Token::Type::Digit};
      tokens.erase(tokens.begin() + i - 1); // delete lhs
      tokens.erase(tokens.begin() + i);     // delete rhs
      i -= 2;
    }
  }

  for (Token& t : tokens) {
    std::cout << "[" << t.lexeme << ", " << t.type << "]\n";
  }
}
