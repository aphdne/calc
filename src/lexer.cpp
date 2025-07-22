#include "lexer.h"

void Lexer::scan(std::string_view p_str) {
  std::string lexeme{};

  for (auto i = 0; i < p_str.size(); i++) {
    char ch = p_str.at(i);

    if (ch != ' ')
      lexeme += ch;

    if (ch == ' ' || i == p_str.size()-1) {
      m_tokens.push_back(Token(lexeme));
      lexeme = "";
    }
  }
}

void Lexer::evaluate() {
  if (m_tokens[0] == "clear") {
    system("clear");
  } else if (m_tokens[0] == "print") {
    m_tokens.erase(m_tokens.begin());
    for (Token& t : m_tokens) {
      std::cout << "\t" << t << std::endl;
    }
    std::cout << ">>> ";
  }
}

void Lexer::clear() {
  m_tokens.clear();
}
