#include <iostream>
#include <print>
#include "lexer.h"

int main() {
  Lexer lexer{};
  std::string statement;

  std::print(">>> ");
  while (std::getline(std::cin, statement)) {
    std::print(">>> ");
    lexer.scan(statement);
    lexer.evaluate();
    lexer.clear();
  }
  std::print("exiting...\n");
}
