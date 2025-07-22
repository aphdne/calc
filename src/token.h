#ifndef TOKEN_H
#define TOKEN_H

#include <ostream>

class Token {
public:
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

  Token(std::string_view p_lexeme);

  std::string_view   get_lexeme() const;
  const Type&        get_type() const;

private:
  static Type get_lexeme_type(std::string_view p_lexeme);

  const std::string m_lexeme;
  const Type m_type;
};

std::ostream& operator<<(std::ostream& p_out, const Token& p_token);

bool operator==(std::string_view p_str, const Token& p_token);
bool operator==(const Token& p_token, Token::Type p_type);
bool operator+=(std::string_view p_str, const Token& p_token);

#endif /* TOKEN_H */
