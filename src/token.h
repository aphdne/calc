#ifndef TOKEN_H
#define TOKEN_H

#include <ostream>

struct Token {
  enum Type {
    Undefined = 0,
    /* multi-char */
    Identifier,
    Digit,
    /* single-char */
    OpenParen,  // (
    CloseParen, // )
    EndLine,    // ;
    /* single-char operators */
    Divide,     // /
    Multiply,   // *
    Minus,      // -
    Plus,       // +
    Assign,     // =
  };

  bool          operator==(std::string_view p_str);
  bool          operator==(Token::Type      p_type);

  std::string lexeme;
  Type type;
};

std::ostream& operator<<(std::ostream&    p_out, const Token& p_token);
std::ostream& operator<<(std::ostream&    p_out, const Token::Type& p_token_type);

bool is_single_char_type(Token::Type p_type);
bool is_operator_type(Token::Type p_type);

#endif /* TOKEN_H */
