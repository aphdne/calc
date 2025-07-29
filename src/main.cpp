#include <iostream>
#include <vector>

#define PRINTC(x) std::cout << #x": " << x << ",\t";
#define PRINTN(x) std::cout << #x": " << x << "\n";

struct Token {
  enum Type {
    Undefined = 0,
    Digit,
    Plus,
    Minus,
    Divide,
    Multiply,
  };

  std::string lexeme;
  Type type;
};

bool inline is_number(char ch) {
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

int main() {
  std::string statement{"55 + -5+"};
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
        if (i < statement.size() - 1 && is_number(statement.at(i+1)))
          curr_type = Token::Type::Digit;
        break;
      case '-': curr_type = Token::Type::Minus;
        if (i < statement.size() - 1 && is_number(statement.at(i+1)))
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

    PRINTC(lexeme);
    PRINTC(ch);
    PRINTC(curr_type);
    PRINTN(prev_type);

    prev_type = curr_type;
  }

  for (Token& t : tokens) {
    std::cout << "[" << t.lexeme << ", " << t.type << "]\n";
  }
}
