#include "token.h"

Token::Token(std::string_view p_str)
     : lexeme{p_str}, type{get_type(p_str)} {}

Token::Type Token::get_type(std::string_view p_str) {
  if (p_str == "(") return Token::OpenParen;
  if (p_str == ")") return Token::CloseParen;
  if (p_str == "/") return Token::Divide;
  if (p_str == "*") return Token::Multiply;
  if (p_str == "-") return Token::Minus;
  if (p_str == "+") return Token::Plus;

  bool digit = true;
  for (char c : p_str) {
    if (c < '0' || c > '9')
      digit = false;
  }
  if (digit) return Token::Digit;

  return Token::Undefined;
}

std::ostream& operator<<(std::ostream& p_out, const Token& p_token) {
  std::string type_string{};
  switch (p_token.type) {
    case Token::Undefined:  type_string = "UNDEFINED";  break;
    case Token::Identifier: type_string = "IDENTIFIER"; break;
    case Token::Digit:      type_string = "DIGIT";      break;
    case Token::OpenParen:  type_string = "OPENPAREN";  break;
    case Token::CloseParen: type_string = "CLOSEPAREN"; break;
    case Token::Divide:     type_string = "DIVIDE";     break;
    case Token::Multiply:   type_string = "MULTIPLY";   break;
    case Token::Minus:      type_string = "MINUS";      break;
    case Token::Plus:       type_string = "PLUS";       break;
  }
  return p_out << "[" << p_token.lexeme << ", " << type_string << "]";
}

bool Token::operator==(std::string_view p_str) {
  return p_str == lexeme;
}

bool Token::operator==(Token::Type p_type) {
  return p_type == type;
}
