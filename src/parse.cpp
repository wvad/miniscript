#include "token.hpp"
#include "node.hpp"
#include <string>

void except(Token &token, const char *expected) {
  if (token.value == expected) return;
  std::cerr << "Unexpected token: " << token.value << "\n  at " << token.file << ":" << token.line << ":" << token.column << "\n";
  exit(1);
}

ExpressionNode *parseExpression(std::vector<Token> &tokens);

ExpressionNode *parseValueExpression(std::vector<Token> &tokens) {
  if (!tokens.size()) {
    std::cerr << "Error: Unexpected end of file\n";
    exit(1);
  }
  if (tokens[0].value == "(") {
    tokens.erase(tokens.begin());
    ExpressionNode *expr = parseExpression(tokens);
    if (!tokens.size()) {
      std::cerr << "Error: Unexpected end of file\n";
      exit(1);
    }
    except(tokens[0], ")");
    tokens.erase(tokens.begin());
    return expr;
  }
  if (tokens[0].kind == TokenKind::NUMBER) {
    NumberNode *node = new NumberNode(std::stod(tokens[0].value));
    tokens.erase(tokens.begin());
    return node;
  }
  if (tokens[0].kind == TokenKind::STRING) {
    StringNode *node = new StringNode(tokens[0].value);
    tokens.erase(tokens.begin());
    return node;
  }
  if (tokens[0].kind == TokenKind::IDENTIFIER) {
    IdentifierNode *node = new IdentifierNode(tokens[0].value);
    tokens.erase(tokens.begin());
    return node;
  }
  if (tokens[0].value == "{") {
    tokens.erase(tokens.begin());
    std::map<std::string, ExpressionNode*> members;
    if (!tokens.size()) {
      std::cerr << "Error: Unexpected end of file\n";
      exit(1);
    }
    if (tokens[0].value != "}") for (;;) {
      if (!tokens.size()) {
        std::cerr << "Error: Unexpected end of file\n";
        exit(1);
      }
      if (tokens[0].kind != TokenKind::IDENTIFIER && tokens[0].kind != TokenKind::STRING) {
        std::cerr << "Error: Unexpected token: " << tokens[0].value << "\n  at " << tokens[0].file << ":" << tokens[0].line << ":" << tokens[0].column << "\n";
        exit(1);
      }
      std::string key = tokens[0].value;
      tokens.erase(tokens.begin());
      if (!tokens.size()) {
        std::cerr << "Error: Unexpected end of file\n";
        exit(1);
      }
      except(tokens[0], ":");
      tokens.erase(tokens.begin());
      members[key] = parseExpression(tokens);
      if (!tokens.size()) {
        std::cerr << "Error: Unexpected end of file\n";
        exit(1);
      }
      if (tokens[0].value == ",") {
        tokens.erase(tokens.begin());
        continue;
      }
      if (tokens[0].value == "}") break;
      std::cerr << "Error: Unexpected token: " << tokens[0].value << "\n  at " << tokens[0].file << ":" << tokens[0].line << ":" << tokens[0].column << "\n";
      exit(1);
    }
    tokens.erase(tokens.begin());
    return new ObjectLiteralNode(members);
  }
  std::cerr << "Unexpected token: " << tokens[0].value << "\n  at " << tokens[0].file << ":" << tokens[0].line << ":" << tokens[0].column << "\n";
  exit(1);
}

ExpressionNode *parsePrimaryExpression(std::vector<Token> &tokens) {
  ExpressionNode *node = parseValueExpression(tokens);
  for (;;) {
    if (!tokens.size()) return node;
    std::string &val = tokens[0].value;
    if (val == ".") {
      tokens.erase(tokens.begin());
      if (!tokens.size()) {
        std::cerr << "Error: Unexpected end of file\n";
        exit(1);
      }
      if (tokens[0].kind != TokenKind::IDENTIFIER) {
        std::cerr << "Expected identifier after '.'\n  at " << tokens[0].file << ":" << tokens[0].line << ":" << tokens[0].column << "\n";
        exit(1);
      }
      node = new MemberAccessNode(node, new StringNode(tokens[0].value));
      tokens.erase(tokens.begin());
    } else if (val == "[") {
      tokens.erase(tokens.begin());
      node = new MemberAccessNode(node, parseExpression(tokens));
      if (!tokens.size()) {
        std::cerr << "Error: Unexpected end of file\n";
        exit(1);
      }
      except(tokens[0], "]");
      tokens.erase(tokens.begin());
    } else if (val == "(") {
      tokens.erase(tokens.begin());
      if (!tokens.size()) {
        std::cerr << "Error: Unexpected end of file\n";
        exit(1);
      }
      std::vector<ExpressionNode*> args;
      if (!tokens.size()) {
        std::cerr << "Error: Unexpected end of file\n";
        exit(1);
      }
      if (tokens[0].value != ")") for (;;) {
        args.push_back(parseExpression(tokens));
        if (!tokens.size()) {
          std::cerr << "Error: Unexpected end of file\n";
          exit(1);
        }
        if (tokens[0].value == ",") {
          tokens.erase(tokens.begin());
          continue;
        }
        if (tokens[0].value == ")") break;
        std::cerr << "Error: Unexpected token: " << tokens[0].value << "\n  at " << tokens[0].file << ":" << tokens[0].line << ":" << tokens[0].column << "\n";
        exit(1);
      }
      tokens.erase(tokens.begin());
      node = new FunctionCallNode(node, args);
    } else {
      return node;
    }
  }
}

ExpressionNode *parseUnaryExpression(std::vector<Token> &tokens) {
  if (tokens.size()) {
    if (tokens[0].value == "!") {
      tokens.erase(tokens.begin());
      return new LogicalNotNode(parsePrimaryExpression(tokens));
    }
    if (tokens[0].value == "typeof") {
      tokens.erase(tokens.begin());
      return new TypeofNode(parsePrimaryExpression(tokens));
    }
  }
  return parsePrimaryExpression(tokens);
}

ExpressionNode *parsePowExpression(std::vector<Token> &tokens) {
  ExpressionNode *left = parseUnaryExpression(tokens);
  if (tokens.size() && tokens[0].value == "**") {
    tokens.erase(tokens.begin());
    return new PowerNode(left, parsePowExpression(tokens));
  }
  return left;
}

ExpressionNode *parseUnaryMinusExpression(std::vector<Token> &tokens) {
  if (tokens.size() && tokens[0].value == "-") {
    tokens.erase(tokens.begin());
    return new UnaryMinusNode(parsePowExpression(tokens));
  }
  return parsePowExpression(tokens);
}

ExpressionNode *parseMulDivExpression(std::vector<Token> &tokens) {
  ExpressionNode *node = parseUnaryMinusExpression(tokens);
  for (;;) {
    if (!tokens.size()) return node;
    std::string &val = tokens[0].value;
    if (val == "*") {
      tokens.erase(tokens.begin());
      node = new MultiplicationNode(node, parseUnaryMinusExpression(tokens));
    } else if (val == "/") {
      tokens.erase(tokens.begin());
      node = new DivisionNode(node, parseUnaryMinusExpression(tokens));
    } else if (val == "%") {
      tokens.erase(tokens.begin());
      node = new RemainderNode(node, parseUnaryMinusExpression(tokens));
    } else {
      return node;
    }
  }
}

ExpressionNode *parseAddSubExpression(std::vector<Token> &tokens) {
  ExpressionNode *node = parseMulDivExpression(tokens);
  for (;;) {
    if (!tokens.size()) return node;
    std::string &val = tokens[0].value;
    if (val == "+") {
      tokens.erase(tokens.begin());
      node = new AdditionNode(node, parseMulDivExpression(tokens));
    } else if (val == "-") {
      tokens.erase(tokens.begin());
      node = new SubtractionNode(node, parseMulDivExpression(tokens));
    } else {
      return node;
    }
  }
}

ExpressionNode *parseRelationalExpression(std::vector<Token> &tokens) {
  ExpressionNode *node = parseAddSubExpression(tokens);
  for (;;) {
    if (!tokens.size()) return node;
    std::string &val = tokens[0].value;
    if (val == "<") {
      tokens.erase(tokens.begin());
      node = new LessThanNode(node, parseAddSubExpression(tokens));
    } else if (val == ">") {
      tokens.erase(tokens.begin());
      node = new GreaterThanNode(node, parseAddSubExpression(tokens));
    } else if (val == "<=") {
      tokens.erase(tokens.begin());
      node = new LessThanOrEqualNode(node, parseAddSubExpression(tokens));
    } else if (val == ">=") {
      tokens.erase(tokens.begin());
      node = new GreaterThanOrEqualNode(node, parseAddSubExpression(tokens));
    } else {
      return node;
    }
  }
}

ExpressionNode *parseEqualityExpression(std::vector<Token> &tokens) {
  ExpressionNode *node = parseRelationalExpression(tokens);
  for (;;) {
    if (!tokens.size()) return node;
    std::string &val = tokens[0].value;
    if (val == "==") {
      tokens.erase(tokens.begin());
      node = new EqualityNode(node, parseRelationalExpression(tokens));
    } else if (val == "!=") {
      tokens.erase(tokens.begin());
      node = new InequalityNode(node, parseRelationalExpression(tokens));
    } else {
      return node;
    }
  }
}

ExpressionNode *parseLogicalAndExpression(std::vector<Token> &tokens) {
  ExpressionNode *left = parseEqualityExpression(tokens);
  while (tokens.size() && tokens[0].value == "&&") {
    tokens.erase(tokens.begin());
    ExpressionNode *right = parseEqualityExpression(tokens);
    left = new LogicalAndNode(left, right);
  }
  return left;
}

ExpressionNode *parseLogicalOrExpression(std::vector<Token> &tokens) {
  ExpressionNode *left = parseLogicalAndExpression(tokens);
  while (tokens.size() && tokens[0].value == "||") {
    tokens.erase(tokens.begin());
    ExpressionNode *right = parseLogicalAndExpression(tokens);
    left = new LogicalOrNode(left, right);
  }
  return left;
}

ExpressionNode *parseConditionalExpression(std::vector<Token> &tokens){
  ExpressionNode *left = parseLogicalOrExpression(tokens);
  if (tokens.size() && tokens[0].value == "?") {
    tokens.erase(tokens.begin());
    ExpressionNode *middle = parseConditionalExpression(tokens);
    if (!tokens.size()) {
      std::cerr << "Error: Unexpected end of file\n";
      exit(1);
    }
    except(tokens[0], ":");
    tokens.erase(tokens.begin());
    return new ConditionalNode(left, middle, parseConditionalExpression(tokens));
  }
  return left;
}

ExpressionNode *parseAssignmentExpression(std::vector<Token> &tokens) {
  ExpressionNode *left = parseConditionalExpression(tokens);
  if (!tokens.size()) return left;
  if (tokens[0].value == "=") {
    tokens.erase(tokens.begin());
    return new AssignmentNode(left, parseAssignmentExpression(tokens));
  }
  if (tokens[0].value == "+=") {
    tokens.erase(tokens.begin());
    return new AssignmentNode(left, new AdditionNode(left, parseAssignmentExpression(tokens)));
  }
  if (tokens[0].value == "-=") {
    tokens.erase(tokens.begin());
    return new AssignmentNode(left, new SubtractionNode(left, parseAssignmentExpression(tokens)));
  }
  if (tokens[0].value == "*=") {
    tokens.erase(tokens.begin());
    return new AssignmentNode(left, new MultiplicationNode(left, parseAssignmentExpression(tokens)));
  }
  if (tokens[0].value == "/=") {
    tokens.erase(tokens.begin());
    return new AssignmentNode(left, new DivisionNode(left, parseAssignmentExpression(tokens)));
  }
  if (tokens[0].value == "%=") {
    tokens.erase(tokens.begin());
    return new AssignmentNode(left, new RemainderNode(left, parseAssignmentExpression(tokens)));
  }
  if (tokens[0].value == "&&=") {
    tokens.erase(tokens.begin());
    return new LogicalAndNode(left, new AssignmentNode(left, parseAssignmentExpression(tokens)));
  }
  if (tokens[0].value == "||=") {
    tokens.erase(tokens.begin());
    return new LogicalOrNode(left, new AssignmentNode(left, parseAssignmentExpression(tokens)));
  }
  return left;
}

ExpressionNode *parseExpression(std::vector<Token> &tokens) {
  return parseAssignmentExpression(tokens);
}

StatementNode *parseStatement(std::vector<Token> &tokens) {
  if (tokens[0].value == "var") {
    tokens.erase(tokens.begin());
    if (!tokens.size()) {
      std::cerr << "Error: Unexpected end of file\n";
      exit(1);
    }
    if (tokens[0].kind != TokenKind::IDENTIFIER) {
      std::cerr << "Error: Expected identifier\n";
      exit(1);
    }
    std::string name = tokens[0].value;
    tokens.erase(tokens.begin());
    if (!tokens.size()) {
      std::cerr << "Error: Unexpected end of file\n";
      exit(1);
    }
    except(tokens[0], "=");
    tokens.erase(tokens.begin());
    auto node = new VariableDeclarationNode(name, parseExpression(tokens));
    except(tokens[0], ";");
    tokens.erase(tokens.begin());
    return node;
  }
  if (tokens[0].value == "while") {
    tokens.erase(tokens.begin());
    ExpressionNode *condition = parseExpression(tokens);
    if (!tokens.size()) {
      std::cerr << "Error: Unexpected end of file\n";
      exit(1);
    }
    except(tokens[0], ":");
    tokens.erase(tokens.begin());
    StatementNode *body = parseStatement(tokens);
    return new WhileNode(condition, body);
  }
  if (tokens[0].value == "if") {
    tokens.erase(tokens.begin());
    ExpressionNode *condition = parseExpression(tokens);
    if (!tokens.size()) {
      std::cerr << "Error: Unexpected end of file\n";
      exit(1);
    }
    except(tokens[0], ":");
    tokens.erase(tokens.begin());
    StatementNode *body = parseStatement(tokens);
    if (!tokens.size() || tokens[0].value != "else") return new IfNode(condition, body);
    tokens.erase(tokens.begin());
    StatementNode *elseBody = parseStatement(tokens);
    return new IfNode(condition, body, elseBody);
  }
  if (tokens[0].value == "break") {
    tokens.erase(tokens.begin());
    if (!tokens.size()) {
      std::cerr << "Error: Unexpected end of file\n";
      exit(1);
    }
    except(tokens[0], ";");
    tokens.erase(tokens.begin());
    return new BreakNode();
  }
  if (tokens[0].value == "continue") {
    tokens.erase(tokens.begin());
    if (!tokens.size()) {
      std::cerr << "Error: Unexpected end of file\n";
      exit(1);
    }
    except(tokens[0], ";");
    tokens.erase(tokens.begin());
    return new ContinueNode();
  }
  if (tokens[0].value == "return") {
    tokens.erase(tokens.begin());
    ExpressionNode *value = parseExpression(tokens);
    if (!tokens.size()) {
      std::cerr << "Error: Unexpected end of file\n";
      exit(1);
    }
    except(tokens[0], ";");
    tokens.erase(tokens.begin());
    return new ReturnNode(value);
  }
  if (tokens[0].value == "fn") {
    tokens.erase(tokens.begin());
    if (!tokens.size()) {
      std::cerr << "Error: Unexpected end of file\n";
      exit(1);
    }
    if (tokens[0].kind != TokenKind::IDENTIFIER) {
      std::cerr << "Error: Expected identifier\n";
      exit(1);
    }
    std::string name = tokens[0].value;
    tokens.erase(tokens.begin());
    if (!tokens.size()) {
      std::cerr << "Error: Unexpected end of file\n";
      exit(1);
    }
    except(tokens[0], "(");
    tokens.erase(tokens.begin());
    std::vector<std::string> parameters;
    if (!tokens.size()) {
      std::cerr << "Error: Unexpected end of file\n";
      exit(1);
    }
    if (tokens[0].value != ")") for (;;) {
      if (!tokens.size()) {
        std::cerr << "Error: Unexpected end of file\n";
        exit(1);
      }
      if (tokens[0].kind != TokenKind::IDENTIFIER) {
        std::cerr << "Error: Expected identifier\n";
        exit(1);
      }
      parameters.push_back(tokens[0].value);
      tokens.erase(tokens.begin());
      if (!tokens.size()) {
        std::cerr << "Error: Unexpected end of file\n";
        exit(1);
      }
      if (tokens[0].value == ",") {
        tokens.erase(tokens.begin());
        continue;
      }
      if (tokens[0].value == ")") break;
      std::cerr << "Error: Unexpected token: " << tokens[0].value << "\n  at " << tokens[0].file << ":" << tokens[0].line << ":" << tokens[0].column << "\n";
      exit(1);
    }
    tokens.erase(tokens.begin());
    return new FunctionDeclarationNode(name, parameters, parseStatement(tokens));
  }
  if (tokens[0].value == "{") {
    tokens.erase(tokens.begin());
    std::vector<StatementNode*> statements;
    if (!tokens.size()) {
      std::cerr << "Error: Unexpected end of file\n";
      exit(1);
    }
    while (tokens[0].value != "}") {
      statements.push_back(parseStatement(tokens));
      if (!tokens.size()) {
        std::cerr << "Error: Unexpected end of file\n";
        exit(1);
      }
    }
    tokens.erase(tokens.begin());
    return new BlockNode(statements);
  }
  ExpressionNode* expr = parseExpression(tokens);
  if (!tokens.size()) {
    std::cerr << "Error: Unexpected end of file\n";
    exit(1);
  }
  except(tokens[0], ";");
  tokens.erase(tokens.begin());
  return new ExpressionStatementNode(expr);
}
