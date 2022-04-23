#include "main.hpp"
#include <set>

std::string numChars = "0123456789";
std::string oneCharSymbols = "+-*/%(){}[].<>=!?,.:;";
std::set<std::string> reserved = {
  "var", "while", "if", "else", "break", "continue", "return", "fn", "typeof", "keys", "empty"
};
std::vector<std::string> multiCharSymbols = {
  "&&=", "||=", "**=", "==", "!=", "<=", ">=", "&&", "||", "**", "+=", "-=", "*=", "/=", "%="
};

void parse(const char *source, std::vector<Token> &tokens) {
  std::uint32_t line = 1, column = 1;
  while (*source) {
    if (*source == ' ' || *source == '\t') {
      source++;
      column++;
      continue;
    }
    if (*source == '\n') {
      source++;
      line++;
      column = 1;
      continue;
    }
    if (*source == '\r' && source[1] == '\n') {
      source += 2;
      line++;
      column = 1;
      continue;
    }
    if (*source == '/' && source[1] == '/') {
      while (*source && *source != '\n') source++;
      line++;
      column = 1;
      continue;
    }
    if (*source == '/' && source[1] == '*') {
      source += 2;
      column += 2;
      for (;;) {
        if (!*source) {
          std::cout << "unterminated comment\n";
          exit(1);
        }
        if (*source == '*' && source[1] == '/') {
          source += 2;
          column += 2;
          break;
        }
        if (*source == '\n') {
          source++;
          line++;
          column = 1;
          continue;
        }
        if (*source == '\r' && source[1] == '\n') {
          source += 2;
          line++;
          column = 1;
          continue;
        }
        source++;
        column++;
      }
      continue;
    }
    if ('0' <= *source && *source <= '9') {
      std::uint32_t len = 1;
      bool includesPoint = *source == '.', includesExp = false;
      while (source[len]) {
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
      std::vector<char> chars;
      while (source[len] != '"') {
        if (!source[len] || source[len] == '\n') {
          std::cout << "Unterminated string\n  at " << line << ":" << column << "\n";
          exit(1);
        }
        if (source[len] != '\\') {
          chars.push_back(source[len]);
          len++;
          continue;
        }
        len++;
        switch (source[len]) {
          case 'n':
            chars.push_back('\n');
            break;
          case 'r':
            chars.push_back('\r');
            break;
          case 'b':
            chars.push_back('\b');
            break;
          case 't':
            chars.push_back('\t');
            break;
          case '\\':
            chars.push_back('\\');
            break;
          case '"':
            chars.push_back('"');
            break;
          default:
            std::cout << "Invalid escape sequence\n  at " << line << ":" << column << "\n";
            exit(1);
        }
        len++;
      }
      tokens.push_back(Token(std::string(chars.begin(), chars.end()), line, column, TokenKind::STRING));
      len++;
      column += len;
      source += len;
      continue;
    }
    if (('A' <= *source && *source <= 'Z') || *source == '_' || ('a' <= *source && *source <= 'z')) {
      std::uint32_t len = 1;
      while (source[len]) {
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
    std::cerr << "Unexpected character\n  at " << line << ":" << column << "\n";
    exit(1);
    continue_label:;
  }
}
