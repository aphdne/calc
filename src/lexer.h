#ifndef LEXER_H
#define LEXER_H

#include <vector>
#include "token.h"

class Lexer {
public:
  void scan(std::string_view p_str);
  void evaluate();
  void clear();

private:
  std::vector<Token> m_tokens;
};

#endif /* LEXER_H */
