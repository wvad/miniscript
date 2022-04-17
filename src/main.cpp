#include "main.hpp"

std::vector<std::string> kinds = {
  "RESERVED",
  "STRING",
  "NUMBER",
  "IDENTIFIER",
  "SYMBOL"
};

void parseExpression(std::vector<Token> &tokens);

int main() {
  std::vector<Token> tokens;
  parse("u.7", tokens);
  for (auto &token : tokens) {
    std::cout << token.value << " (" << token.line << ":" << token.column << ") " << kinds[static_cast<int>(token.kind)] << "\n";
  }
  parseExpression(tokens);
  std::cout << "remain:\n";
  for (auto &token : tokens) {
    std::cout << token.value << " (" << token.line << ":" << token.column << ") " << kinds[static_cast<int>(token.kind)] << "\n";
  }
  std::cout << "end\n";
  return 0;
}
