#include "lexer.h"

constexpr std::string prompt{"\e[1;34m>>> \e[0m"};

int main() {
  Lexer lexer{};
  std::string statement{};

  std::cout << prompt;
  while (std::getline(std::cin, statement)) {
    if (statement[statement.size()-1] != ';')
      statement += ';';

    lexer.tokenise(statement);
    lexer.evaluate();

    std::cout << prompt;
  }
  std::cout << "exiting...\n";
}
