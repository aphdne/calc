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
};

std::ostream& operator<<(std::ostream& p_out, const Token& p_token);

bool operator==(std::string_view p_str, const Token& p_token);
bool operator==(const Token& p_token, Token::Type p_type);

#endif /* TOKEN_H */
