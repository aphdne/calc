#include "token.h"

std::ostream& Token::operator<<(std::ostream& p_out) {
  std::string type_string{};
  switch (type) {
    case Token::Undefined:  type_string = "UNDEFINED";  break;
    case Token::Identifier: type_string = "IDENTIFIER"; break;
    case Token::LeftParen:  type_string = "LEFTPAREN";  break;
    case Token::RightParen: type_string = "RIGHTPAREN"; break;
    case Token::Divide:     type_string = "DIVIDE";     break;
    case Token::Multiply:   type_string = "MULTIPLY";   break;
    case Token::Minus:      type_string = "MINUS";      break;
    case Token::Plus:       type_string = "PLUS";       break;
  }
  return p_out << "[" << lexeme << ", " << type_string << "]";
}

bool Token::operator==(std::string_view p_str) {
  return p_str == lexeme;
}

bool Token::operator==(Token::Type p_type) {
  return p_type == type;
}
