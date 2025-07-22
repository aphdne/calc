#include "token.h"

// Token::Token(std::string_view p_lexeme)
//   : m_lexeme{p_lexeme}, m_type{get_lexeme_type(p_lexeme)} {
// }

// std::string_view Token::get_lexeme() const {
//   return m_lexeme;
// }

// const Token::Type& Token::get_type() const {
//   return m_type;
// }

// Token::Type Token::get_lexeme_type(std::string_view p_lexeme) {
//   if (p_lexeme == "+")
//     return Token::Plus;
//   return Token::Identifier;
// }

std::ostream& operator<<(std::ostream& p_out, const Token& p_token) {
  std::string type_string{};
  switch (p_token.type) {
    case Token::Undefined:  type_string = "UNDEFINED";  break;
    case Token::Identifier: type_string = "IDENTIFIER"; break;
    case Token::LeftParen:  type_string = "LEFTPAREN";  break;
    case Token::RightParen: type_string = "RIGHTPAREN"; break;
    case Token::Divide:     type_string = "DIVIDE";     break;
    case Token::Multiply:   type_string = "MULTIPLY";   break;
    case Token::Minus:      type_string = "MINUS";      break;
    case Token::Plus:       type_string = "PLUS";       break;
  }
  return p_out << "[" << p_token.lexeme << ", " << type_string << "]";
}

bool operator==(std::string_view p_str, const Token& p_token) {
  return p_str == p_token.lexeme;
}

bool operator==(const Token& p_token, Token::Type p_type) {
  return p_type == p_token.type;
}
