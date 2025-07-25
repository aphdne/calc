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

void Lexer::operate(int p_operator) {
  int li{p_operator-1}, ri{p_operator+1};

  for (; li > 0, m_tokens[li] != Token::Type::Digit; li--) {
    if (m_tokens[li] != Token::Type::Undefined)
      error("operators require a digit on their left side");
  }

  for (; ri > 0, m_tokens[ri] != Token::Type::Digit; ri++) {
    if (m_tokens[ri] != Token::Type::Undefined)
      error("operators require a digit on their right side");
  }

  m_tokens[p_operator] = calculate(m_tokens[li], m_tokens[ri], m_tokens[p_operator]);
  m_tokens[li]={"DELETED", Token::Type::Undefined};
  m_tokens[ri]={"DELETED", Token::Type::Undefined};
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

  std::vector<std::pair<int,int>> parentheses{};
  int close_amt{};
  for (int i = 0; i < m_tokens.size(); i++) {
    if (m_tokens[i].type == Token::Type::OpenParen) {
      int open_amt{1};
      for (int j = i+1; j < m_tokens.size(); j++) {
        if (m_tokens[j] == Token::Type::OpenParen) {
          open_amt++;
        } else if (m_tokens[j] == Token::Type::CloseParen) {
          close_amt--;
          open_amt--;
          if (open_amt == 0) {
            parentheses.push_back({i, j});
            break;
          }
        }
      }
      if (open_amt > 0) {
        error("unmatched open parenthesis '('");
        clear();
        return;
      }
    } else if (m_tokens[i].type == Token::Type::CloseParen) {
      close_amt++;
    }
  }
  if (close_amt > 0) {
    error("unmatched close parenthesis ')'");
    clear();
    return;
  }

  // for (std::pair<int, int> p : parentheses) {
  //   std::cout << "[" << p.first << ", " << p.second << "]\n";
  // }

  std::vector<int> operators{};
  for (int i = 0; i < m_tokens.size(); i++) {
    if (is_operator_type(m_tokens[i].type)) {
      operators.push_back(i);
    }
  }

  for (const std::pair<int, int>& p : parentheses) {
    // for (int o : operators) {
    for (int i = 0; i < operators.size(); i++) {
      int o = operators[i];
      if (o > p.first && o < p.second) {
        operate(o);
        operators.erase(operators.begin() + i);
      }
      m_tokens[p.first]={"DELETED", Token::Type::Undefined};
      m_tokens[p.second]={"DELETED", Token::Type::Undefined};
    }
  }

  for (int o : operators) {
    operate(o);
  }

  for (const Token& t : m_tokens) {
    if (t.type == Token::Type::Undefined)
      continue;
    std::cout << t.lexeme << "\n";
  }

  clear();
}

void Lexer::clear() {
  m_tokens.clear();
}

void Lexer::print() const {
  for (const Token& t : m_tokens) {
    std::cout << t << std::endl;
  }
}
