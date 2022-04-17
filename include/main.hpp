#include <iostream>
#include <vector>

enum class TokenKind {
  RESERVED,
  STRING,
  NUMBER,
  IDENTIFIER,
  SYMBOL
};

struct Token {
  std::string value;
  std::uint32_t line;
  std::uint32_t column;
  TokenKind kind;
  std::string file = "unknown.ms";
  Token(std::string value, std::uint32_t line, std::uint32_t column, TokenKind kind)
      : value(std::move(value)), line(line), column(column), kind(kind) {}
};

void parse(const char *source, std::vector<Token> &tokens);
