#include "lexer.h"

static inline void error(std::string_view p_errmsg) {
  std::cout << "\e[1;31m[error] " << p_errmsg << "\e[0m\n";
}

static inline void alert(std::string_view p_errmsg) {
  std::cout << "\e[1;33m[alert] " << p_errmsg << "\e[0m\n";
}

static inline void del(Token& p_token) {
  p_token.type = Token::Type::Undefined;
  p_token.lexeme = "";
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
        alert("symbol \"" + lexeme + "\" is undefined; ignoring");
      lexeme = "";
    }
    lexeme += ch;

    prev_type = current_type;
  }
}

void Lexer::operate(int p_operator) {
  int l_i{p_operator-1}, r_i{p_operator+1};

  for (; l_i > 0, m_tokens[l_i] != Token::Type::Digit; l_i--) {
    if (m_tokens[l_i] != Token::Type::Undefined)
      error("operators require a digit on their left side");
  }

  for (; r_i > 0, m_tokens[r_i] != Token::Type::Digit; r_i++) {
    if (m_tokens[r_i] != Token::Type::Undefined)
      error("operators require a digit on their r_ight side");
  }

  switch (m_tokens[p_operator].type) {
  case Token::Type::Divide:
    m_tokens[p_operator] = Token(p_lhs.to_int() / p_rhs.to_int());
    break;
  case Token::Type::Multiply:
    m_tokens[p_operator] = Token(p_lhs.to_int() * p_rhs.to_int());
    break;
  case Token::Type::Plus:
    m_tokens[p_operator] = Token(p_lhs.to_int() + p_rhs.to_int());
    break;
  case Token::Type::Minus:
    m_tokens[p_operator] = Token(p_lhs.to_int() - p_rhs.to_int());
    break;
  }

  del(m_tokens[l_i]);
  del(m_tokens[r_i]);
}

// TODO: identifiers
void Lexer::evaluate() {
  if (m_tokens.empty())
    return;

  if (m_tokens[0] == "clear") {
    system("clear");
    return;
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

  std::vector<int> operators{};
  for (int i = 0; i < m_tokens.size(); i++)
    if (is_operator_type(m_tokens[i].type))
      operators.push_back(i);

  // sort operators - follows the enum defined in token.h
  std::sort(operators.begin(), operators.end(), [this](int a, int b) {
                                                  return m_tokens[a].type < m_tokens[b].type;
                                                });

  for (const std::pair<int, int>& p : parentheses) {
    for (int i = 0; i < operators.size(); i++) {
      int o = operators[i];
      if (o > p.first && o < p.second) {
        operate(o);
        operators.erase(operators.begin() + i);
      }
      del(m_tokens[p.first]);
      del(m_tokens[p.second]);
    }
  }

  for (int o : operators) {
    operate(o);
  }

  // by this point m_tokens should be populated by mostly undefined tokens & all operations should have converged into one token
  // so we print that single undefined token as our answer & clear m_tokens
  for (const Token& t : m_tokens) {
    if (t.type == Token::Type::Undefined)
      continue;
    std::cout << t.lexeme << "\n";
    break;
  }

  clear();
}

void Lexer::clear() {
  m_tokens.clear();
}

void Lexer::print() const {
  for (const Token& t : m_tokens)
    std::cout << t << std::endl;
}
