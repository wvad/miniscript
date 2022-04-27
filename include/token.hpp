#ifndef __MAIN_H__
#define __MAIN_H__

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
  std::string file;
  Token(std::string value, std::uint32_t line, std::uint32_t column, TokenKind kind, std::string file)
    : value(value), line(line), column(column), kind(kind), file(file) {}
};

void parse(const char *source, std::vector<Token> &tokens, const char *filename);

#endif /* __MAIN_H__ */
