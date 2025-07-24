#include "lexer.h"

static void error(std::string_view p_errmsg) {
  std::cout << "\e[1;31m[error] " << p_errmsg << "\e[0m\n";
}

static void alert(std::string_view p_errmsg) {
  std::cout << "\e[1;33m[alert] " << p_errmsg << "\e[0m\n";
}

static Token calculate(const Token& p_lhs, const Token& p_rhs, const Token& p_op) {
  switch (p_op.type) {
  case Token::Type::Divide:
    return Token(p_lhs.to_int() / p_rhs.to_int());
    break;
  case Token::Type::Multiply:
    return Token(p_lhs.to_int() * p_rhs.to_int());
    break;
  case Token::Type::Plus:
    return Token(p_lhs.to_int() + p_rhs.to_int());
    break;
  case Token::Type::Minus:
    return Token(p_lhs.to_int() - p_rhs.to_int());
    break;
  }
  return { "NaN", Token::Type::Undefined };
}

void Lexer::tokenise(std::string_view p_input) {
  std::string lexeme{};
  Token::Type current_type{get_token_type(p_input.at(0))};
  Token::Type prev_type{current_type};

  for (auto i = 0; i < p_input.size(); i++) {
    char ch = p_input.at(i);

    current_type = get_token_type(ch);

    if (current_type != prev_type || is_single_char_type(current_type) && lexeme != "") {
      if (prev_type != Token::Undefined)
        m_tokens.push_back({ lexeme, prev_type });
      else if (lexeme != " ")
        alert("lexeme \"" + lexeme + "\" is undefined; ignoring");
      lexeme = "";
    }
    lexeme += ch;

    prev_type = current_type;
  }
}

// TODO: order of operations, identifiers
void Lexer::evaluate() {
  if (m_tokens.empty())
    return;

  if (m_tokens[0] == "clear") {
    system("clear");
    return;
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
        if (i == 0 || i == m_tokens.size()-1) {
          error("operations must have a left-hand and right-hand symbol");
          clear();
          return;
        }

        Token& lhs = m_tokens[i-1];
        Token& op  = m_tokens[i];
        Token& rhs = m_tokens[i+1];

        if (lhs.type != Token::Type::Digit || rhs.type != Token::Type::Digit) {
          error("left-hand symbol must be a digit/identifier; right-hand symbol must be a digit");
          clear();
          return;
        }

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

void Lexer::print() const {
  for (const Token& t : m_tokens) {
    std::cout << t << std::endl;
  }
}
