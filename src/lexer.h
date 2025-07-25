#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include <iostream>
#include "token.h"

class Lexer {
public:
  void tokenise(std::string_view p_str);
  void evaluate();
  void clear();
  void print() const;
  void operate(int p_operator);

private:
  std::vector<Token> m_tokens;
};

#endif /* LEXER_H */
