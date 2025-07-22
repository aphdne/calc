#ifndef TOKEN_H
#define TOKEN_H

#include <ostream>

struct Token {
  enum Type {
    Undefined   = 0,
    Identifier,
    LeftParen   = '(',
    RightParen  = ')',
    Divide      = '/',
    Multiply    = '*',
    Minus       = '-',
    Plus        = '+',
  };

  const std::string lexeme;
  const Type type;

  std::ostream& operator<<(std::ostream&    p_out);
  bool          operator==(std::string_view p_str);
  bool          operator==(Token::Type      p_type);
};

#endif /* TOKEN_H */
