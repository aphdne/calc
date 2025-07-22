#include "token.h"

std::ostream& operator<<(std::ostream& p_out, const Token& p_token) {
  return p_out << "[" << p_token.lexeme << ", \t" << p_token.type << "]";
}

std::ostream& operator<<(std::ostream&    p_out, const Token::Type& p_token_type) {
  std::string type_string{};
  switch (p_token_type) {
    case Token::Undefined:  type_string = "UNDEFINED";  break;
    case Token::Identifier: type_string = "IDENTIFIER"; break;
    case Token::Digit:      type_string = "DIGIT";      break;
    case Token::OpenParen:  type_string = "OPENPAREN";  break;
    case Token::CloseParen: type_string = "CLOSEPAREN"; break;
    case Token::Divide:     type_string = "DIVIDE";     break;
    case Token::Multiply:   type_string = "MULTIPLY";   break;
    case Token::Minus:      type_string = "MINUS";      break;
    case Token::Plus:       type_string = "PLUS";       break;
    case Token::Assign:     type_string = "ASSIGN";     break;
    case Token::EndLine:    type_string = "ENDLINE";    break;
  }
  return p_out << type_string;
}

bool Token::operator==(std::string_view p_str) {
  return p_str == lexeme;
}

bool Token::operator==(Token::Type p_type) {
  return p_type == type;
}

bool is_single_char_type(Token::Type p_type) {
  return p_type >= Token::Type::OpenParen;
}

bool is_operator_type(Token::Type p_type) {
  return p_type >= Token::Type::Divide;
}
