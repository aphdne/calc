#include <iostream>
#include <print>
#include "lexer.h"

int main() {
  Lexer lexer{};
  std::string statement;

  std::print(">>> ");
  while (std::getline(std::cin, statement)) {
    if (statement[statement.size()-1] != ';')
      statement += ';';

    lexer.scan(statement);

    try {
      lexer.evaluate();
    } catch (Error err) {
      std::print("\e[1;31m[error] ");
      switch (err) {
      case Error::AbsentSymbol:
        std::print("operations must have a left-hand and right-hand symbol");
        break;
      case Error::InvalidSymbol:
        std::print("invalid left-hand or right-hand symbol; must be a digit/identifier");
        break;
      }
      std::print("\e[0m\n");
      lexer.clear();
    }
    std::print(">>> ");
  }
  std::print("exiting...\n");
}
