#ifndef LEXER_H
#define LEXER_H

#include <cmath>
#include <vector>
#include <iostream>
#include "token.h"

enum class Error {
  InvalidSymbol = 0,
  AbsentSymbol  = 1,
};

class Lexer {
public:
  void scan(std::string_view p_str);
  void evaluate();
  void clear();
  void print();

  static Token int_to_token(int p_int);
  static int   token_to_int(const Token& p_token);
private:
  static Token calculate(const Token& lhs, const Token& rhs, const Token& op);

  std::vector<Token> m_tokens;
};

#endif /* LEXER_H */
