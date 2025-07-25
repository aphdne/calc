#ifndef TOKEN_H
#define TOKEN_H

#include <ostream> // operator<<
#include <cmath> // converting Tokens to ints & vice versa

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
    Plus,       // +
    Minus,      // -
    Assign,     // =
  };

  Token(std::string_view p_lexeme, Token::Type p_type);
  Token(int p_int);
  int to_int() const;

  std::string lexeme;
  Type type;
};

std::ostream& operator<<(std::ostream& p_out, const Token& p_token);
std::ostream& operator<<(std::ostream& p_out, const Token::Type& p_token_type);
bool operator==(std::string_view p_str, const Token& p_token);
bool operator==(Token::Type p_type, const Token& p_token);

Token::Type get_token_type(char ch);
bool is_single_char_type(Token::Type p_type);
bool is_operator_type(Token::Type p_type);

#endif /* TOKEN_H */
