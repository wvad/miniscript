#include "token.hpp"
#include "abnode.hpp"

std::vector<std::string> kinds = {
  "RESERVED",
  "STRING",
  "NUMBER",
  "IDENTIFIER",
  "SYMBOL"
};

int main() {
  std::vector<Token> tokens;
  parse("var a = { a: 0, b: 7 };", tokens, "unknown.ms");
  for (auto &token : tokens)
    std::cout << token.value << " (" << token.line << ":" << token.column << ") " << kinds[static_cast<int>(token.kind)] << "\n";
  while (tokens.size()) parseStatement(tokens);
  std::cout << "end\n";
  return 0;
}
