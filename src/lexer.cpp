#include "lexer.h"

void Lexer::scan(std::string_view p_str) {
  auto get_type = [](char ch){
    if      (ch == '(')                           return Token::OpenParen;
    else if (ch == ')')                           return Token::CloseParen;
    else if (ch == '/')                           return Token::Divide;
    else if (ch == '*')                           return Token::Multiply;
    else if (ch == '-')                           return Token::Minus;
    else if (ch == '+')                           return Token::Plus;
    else if (ch == '=')                           return Token::Assign;
    else if (ch == ';')                           return Token::EndLine;
    else if (ch >= '0' && ch <= '9')              return Token::Digit;
    else if (ch >= 'a' && ch <= 'z' || ch == '_') return Token::Identifier;
    else                                          return Token::Undefined;
  };

  std::string lexeme{};
  Token::Type current_type{get_type(p_str.at(0))};
  Token::Type prev_type{current_type};

  for (auto i = 0; i < p_str.size(); i++) {
    char ch = p_str.at(i);

    current_type = get_type(ch);

    if (current_type != prev_type) {
      if (prev_type != Token::Undefined)
        m_tokens.push_back({ lexeme, prev_type });
      lexeme = "";
    }
    lexeme += ch;

    prev_type = current_type;
  }
}

void Lexer::evaluate() {
  if (m_tokens[0] == "clear") {
    system("clear");
  } else if (m_tokens[0] == "print") {
    m_tokens.erase(m_tokens.begin());
    print();
  }
}

void Lexer::clear() {
  m_tokens.clear();
}

void Lexer::print() {
  for (Token& t : m_tokens) {
    std::cout << "\t" << t << std::endl;
  }
  if (!m_tokens.empty())
    std::cout << ">>> ";
}
