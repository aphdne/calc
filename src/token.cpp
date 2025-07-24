#include "token.h"

Token::Token(std::string_view p_lexeme, Token::Type p_type)
: type{p_type}, lexeme{p_lexeme} {}

Token::Token(int p_int)
: type{Token::Type::Digit}, lexeme{} {
  int super{};
  while (std::pow(10, super) <= p_int) {
    super++;
  }

  for (int i = super; i > 0; i--) {
    char c = (p_int / static_cast<int>(std::pow(10, i-1))) % 10 + 48;
    lexeme += c;
  }
}

int Token::to_int() const {
  if (type != Token::Digit)
    throw std::logic_error("token_to_int: requires a digit token type");

  int result{};
  for (int i = 0; i < lexeme.size(); i++) {
    int i_rev = lexeme.size()-1-i;
    int ch    = static_cast<int>(lexeme[i_rev]) - 48; // digit chars begin at 48
    result += ch * std::pow(10, i);
  }
  return result;
}

std::ostream& operator<<(std::ostream& p_out, const Token& p_token) {
  return p_out << "[" << p_token.lexeme << ", \t" << p_token.type << "]";
}

std::ostream& operator<<(std::ostream& p_out, const Token::Type& p_token_type) {
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

bool operator==(std::string_view p_str, const Token& p_token) {
  return p_str == p_token.lexeme;
}

bool operator==(Token::Type p_type, const Token& p_token) {
  return p_type == p_token.type;
}

Token::Type get_token_type(char ch) {
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
}

bool is_single_char_type(Token::Type p_type) {
  return p_type >= Token::Type::OpenParen;
}

bool is_operator_type(Token::Type p_type) {
  return p_type >= Token::Type::Divide;
}
