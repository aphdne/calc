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

    if (current_type != prev_type || is_single_char_type(current_type) && lexeme != "") {
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

  auto operator_present = [this](){
    for (Token& t : m_tokens)
      if (is_operator_type(t.type))
        return true;
    return false;
  };

  while (operator_present()) {
    int size = m_tokens.size();
    for (int i = 0; i < size; i++) {
      if (size != m_tokens.size()) {
        i--;
        size = m_tokens.size();
        continue;
      }

      if (is_operator_type(m_tokens[i].type)) {
        if (i == 0 || i == m_tokens.size()-1)
          throw Error::AbsentSymbol;

        Token& lhs = m_tokens[i-1];
        Token& op  = m_tokens[i];
        Token& rhs = m_tokens[i+1];

        if (lhs.type != Token::Type::Digit)
          throw Error::InvalidSymbol;
        else if (rhs.type != Token::Type::Digit)
          throw Error::InvalidSymbol;

        op = calculate(lhs, rhs, op);
        m_tokens.erase(m_tokens.begin()+i-1);
        m_tokens.erase(m_tokens.begin()+i);
      }
    }
  }

  std::cout << m_tokens[0].lexeme << std::endl;

  m_tokens.clear();
}

void Lexer::clear() {
  m_tokens.clear();
}

void Lexer::print() {
  for (Token& t : m_tokens) {
    std::cout << t << std::endl;
  }
}

int Lexer::token_to_int(const Token& p_token) {
  if (p_token.type != Token::Digit)
    throw std::logic_error("token_to_int: requires a digit token type");

  int result{};
  for (int i = 0; i < p_token.lexeme.size(); i++) {
    int i_rev = p_token.lexeme.size()-1-i;
    int ch    = static_cast<int>(p_token.lexeme[i_rev]) - 48; // digit chars begin at 48
    result += ch * std::pow(10, i);
  }
  return result;
}

Token Lexer::int_to_token(int p_int) {
  std::string s{};

  int super{};
  while (std::pow(10, super) <= p_int) {
    super++;
  }

  for (int i = super; i > 0; i--) {
    int x = std::pow(10, i-1);
    // std::cout << x << ",\t10^" << i-1;
    char c = (p_int / x) % 10 + 48;
    // std::cout << ",\t" << c << "\n";
    s += c;
  }

  return { s, Token::Type::Digit };
}

Token Lexer::calculate(const Token& lhs, const Token& rhs, const Token& op) {
  switch (op.type) {
  case Token::Type::Divide:
    return int_to_token(token_to_int(lhs) / token_to_int(rhs));
    break;
  case Token::Type::Multiply:
    return int_to_token(token_to_int(lhs) * token_to_int(rhs));
    break;
  case Token::Type::Plus:
    return int_to_token(token_to_int(lhs) + token_to_int(rhs));
    break;
  case Token::Type::Minus:
    return int_to_token(token_to_int(lhs) - token_to_int(rhs));
    break;
  }
  return { "-1", Token::Type::Digit };
}
