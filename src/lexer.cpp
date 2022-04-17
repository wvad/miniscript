#include "main.hpp"
#include <set>

std::string numChars = "0123456789";

std::string oneCharSymbols = "+-*/%(){}[].<>=!?,.:;";

std::set<std::string> reserved = { "var", "while", "if", "else", "break", "continue", "return", "fn", "typeof", "keys", "empty" };

std::vector<std::string> multiCharSymbols = { "&&=", "||=", "**=", "==", "!=", "<=", ">=", "&&", "||", "**", "+=", "-=", "*=", "/=", "%=" };

void parse(const char *source, std::vector<Token> &tokens) {
  std::uint32_t line = 1, column = 1;
  while (*source) {
    if (*source == ' ' || *source == '\t') {
      source++;
      column++;
      continue;
    }
    if (*source == '\n' || (*source == '\r' && source[1] == '\n')) {
      source++;
      line++;
      column = 1;
      continue;
    }
    if ('0' <= *source && *source <= '9') {
      std::uint32_t len = 1;
      bool includesPoint = *source == '.',
           includesExp = false;
      for (;;) {
        if ('0' <= source[len] && source[len] <= '9') {
          len++;
          continue;
        }
        if (!includesExp && source[len] == 'e') {
          len++;
          includesExp = true;
          continue;
        }
        if (!includesExp && !includesPoint && source[len] == '.') {
          len++;
          includesPoint = true;
          continue;
        }
        break;
      }
      tokens.push_back(Token(std::string(source, len), line, column, TokenKind::NUMBER));
      column += len;
      source += len;
      continue;
    }
    if (*source == '"') {
      std::uint32_t len = 1;
      while (source[len] != '"') {
        if (source[len] != '\\') {
          len++;
          continue;
        }
        // [TODO]: escaped characters
      }
      tokens.push_back(Token(std::string(source + 1, len - 1), line, column, TokenKind::STRING));
      len++;
      column += len;
      source += len;
      continue;
    }
    if (('A' <= *source && *source <= 'Z') || *source == '_' || ('a' <= *source && *source <= 'z')) {
      std::uint32_t len = 1;
      for (;;) {
        if (('A' <= source[len] && source[len] <= 'Z') || ('0' <= source[len] && source[len] <= '9') || source[len] == '_' || ('a' <= source[len] && source[len] <= 'z')) {
          len++;
          continue;
        }
        break;
      }
      std::string value(source, len);
      tokens.push_back(Token(value, line, column, reserved.find(value) != reserved.end() ? TokenKind::RESERVED : TokenKind::IDENTIFIER));
      column += len;
      source += len;
      continue;
    }
    for (auto &symbol : multiCharSymbols) {
      size_t len = symbol.size();
      if (std::string(source, len) == symbol) {
        tokens.push_back(Token(symbol, line, column, TokenKind::SYMBOL));
        column += len;
        source += len;
        goto continue_label;
      }
    }
    if (oneCharSymbols.find(*source) != std::string::npos) {
      tokens.push_back(Token(std::string(source, 1), line, column, TokenKind::SYMBOL));
      column++;
      source++;
      continue;
    }
    std::cerr << "Unexpected character at " << line << ":" << column << "\n";
    exit(1);
    continue_label:;
  }
}
