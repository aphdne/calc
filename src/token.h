#ifndef TOKEN_H
#define TOKEN_H

#include <ostream>

struct Token {
  enum Type {
    Undefined = 0,
    Identifier,
    Digit,
    OpenParen,
    CloseParen,
    Divide,
    Multiply,
    Minus,
    Plus,
  };

  Token(std::string_view p_str);

  static Type get_type(std::string_view p_str);

  bool          operator==(std::string_view p_str);
  bool          operator==(Token::Type      p_type);

  std::string lexeme;
  Type type;
};

std::ostream& operator<<(std::ostream&    p_out, const Token& p_token);
// bool operator==(const char p_char, const Token::Type p_type);

#endif /* TOKEN_H */
