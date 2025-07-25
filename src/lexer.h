#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <iostream>
#include <algorithm>
#include "token.h"

class Lexer {
public:
  void tokenise(std::string_view p_str);
  void operate(int p_operator);
  void evaluate();
  void clear();
  void print() const;

private:
  std::vector<Token> m_tokens;
};

#endif /* LEXER_H */
