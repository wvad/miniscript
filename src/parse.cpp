#include "main.hpp"
#include <string>

struct Node {};

struct ExpressionNode : Node {};

struct BinaryOperatorNode : ExpressionNode {
  ExpressionNode *left;
  ExpressionNode *right;
  BinaryOperatorNode(ExpressionNode *left, ExpressionNode *right) : left(left), right(right) {}
};

struct AssignmentNode : BinaryOperatorNode {
  AssignmentNode(ExpressionNode *left, ExpressionNode *right) : BinaryOperatorNode(left, right) {}
};

struct ConditionalNode : ExpressionNode {
  ExpressionNode *condition;
  ExpressionNode *trueBranch;
  ExpressionNode *falseBranch;
  ConditionalNode(ExpressionNode *condition, ExpressionNode *trueBranch, ExpressionNode *falseBranch)
      : condition(condition), trueBranch(trueBranch), falseBranch(falseBranch) {}
};

struct LogicalOrNode : BinaryOperatorNode {
  LogicalOrNode(ExpressionNode *left, ExpressionNode *right) : BinaryOperatorNode(left, right) {}
};

struct LogicalAndNode : BinaryOperatorNode {
  LogicalAndNode(ExpressionNode *left, ExpressionNode *right) : BinaryOperatorNode(left, right) {}
};

struct EqualityNode : BinaryOperatorNode {
  EqualityNode(ExpressionNode *left, ExpressionNode *right) : BinaryOperatorNode(left, right) {}
};

struct InequalityNode : BinaryOperatorNode {
  InequalityNode(ExpressionNode *left, ExpressionNode *right) : BinaryOperatorNode(left, right) {}
};

struct LessThanNode : BinaryOperatorNode {
  LessThanNode(ExpressionNode *left, ExpressionNode *right) : BinaryOperatorNode(left, right) {}
};

struct GreaterThanNode : BinaryOperatorNode {
  GreaterThanNode(ExpressionNode *left, ExpressionNode *right) : BinaryOperatorNode(left, right) {}
};

struct LessThanOrEqualNode : BinaryOperatorNode {
  LessThanOrEqualNode(ExpressionNode *left, ExpressionNode *right) : BinaryOperatorNode(left, right) {}
};

struct GreaterThanOrEqualNode : BinaryOperatorNode {
  GreaterThanOrEqualNode(ExpressionNode *left, ExpressionNode *right) : BinaryOperatorNode(left, right) {}
};

struct AdditionNode : BinaryOperatorNode {
  AdditionNode(ExpressionNode *left, ExpressionNode *right) : BinaryOperatorNode(left, right) {}
};

struct SubtractionNode : BinaryOperatorNode {
  SubtractionNode(ExpressionNode *left, ExpressionNode *right) : BinaryOperatorNode(left, right) {}
};

struct MultiplicationNode : BinaryOperatorNode {
  MultiplicationNode(ExpressionNode *left, ExpressionNode *right) : BinaryOperatorNode(left, right) {}
};

struct DivisionNode : BinaryOperatorNode {
  DivisionNode(ExpressionNode *left, ExpressionNode *right) : BinaryOperatorNode(left, right) {}
};

struct RemainderNode : BinaryOperatorNode {
  RemainderNode(ExpressionNode *left, ExpressionNode *right) : BinaryOperatorNode(left, right) {}
};

struct PowerNode : BinaryOperatorNode {
  PowerNode(ExpressionNode *left, ExpressionNode *right) : BinaryOperatorNode(left, right) {}
};

struct UnaryMinusNode : ExpressionNode {
  ExpressionNode *operand;
  UnaryMinusNode(ExpressionNode *operand) : operand(operand) {}
};

struct LogicalNotNode : ExpressionNode {
  ExpressionNode *operand;
  LogicalNotNode(ExpressionNode *operand) : operand(operand) {}
};

struct TypeofNode : ExpressionNode {
  ExpressionNode *operand;
  TypeofNode(ExpressionNode *operand) : operand(operand) {}
};

struct MemberAccessNode : BinaryOperatorNode {
  MemberAccessNode(ExpressionNode *left, ExpressionNode *right) : BinaryOperatorNode(left, right) {}
};

struct FunctionCallNode : ExpressionNode {
  ExpressionNode *callee;
  std::vector<ExpressionNode*> args;
  FunctionCallNode(ExpressionNode *callee, std::vector<ExpressionNode*> args) : callee(callee), args(args) {}
};

struct IdentifierNode : ExpressionNode {
  std::string name;
  IdentifierNode(std::string name) : name(name) {}
};

struct StringNode : ExpressionNode {
  std::string value;
  StringNode(std::string value) : value(value) {}
};

struct NumberNode : ExpressionNode {
  double value;
  NumberNode(double value) : value(value) {}
};

struct ObjectLiteralNode : ExpressionNode {};

void except(Token &token, std::string expected) {
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
  // [TODO]: Object Literal
  std::cerr << "Unexpected token: " << tokens[0].value << "\n  at " << tokens[0].file << ":" << tokens[0].line << ":" << tokens[0].column << "\n";
  exit(1);
}

ExpressionNode *parsePrimaryExpression(std::vector<Token> &tokens) {
  if (tokens.size() && tokens[0].value == "(") {
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
      node = new MemberAccessNode(node, parseExpression(tokens));
      if (!tokens.size()) {
        std::cerr << "Error: Unexpected end of file\n";
        exit(1);
      }
      except(tokens[0], ")");
      tokens.erase(tokens.begin());
    } else {
      return node;
    }
  }
}

ExpressionNode *parsePowExpression(std::vector<Token> &tokens) {
  ExpressionNode *left = parsePrimaryExpression(tokens);
  if (tokens.size() && tokens[0].value == "**") {
    tokens.erase(tokens.begin());
    return new PowerNode(left, parsePowExpression(tokens));
  }
  return left;
}

ExpressionNode *parseUnaryExpression(std::vector<Token> &tokens) {
  ExpressionNode *left = parsePowExpression(tokens);
  if (!tokens.size()) return left;
  if (tokens[0].value == "-") {
    tokens.erase(tokens.begin());
    return new UnaryMinusNode(parseUnaryExpression(tokens));
  }
  if (tokens[0].value == "!") {
    tokens.erase(tokens.begin());
    return new LogicalNotNode(parseUnaryExpression(tokens));
  }
  if (tokens[0].value == "typeof") {
    tokens.erase(tokens.begin());
    return new TypeofNode(parseUnaryExpression(tokens));
  }
  return left;
}

ExpressionNode *parseMulDivExpression(std::vector<Token> &tokens) {
  ExpressionNode *node = parseUnaryExpression(tokens);
  for (;;) {
    if (!tokens.size()) return node;
    std::string &val = tokens[0].value;
    if (val == "*") {
      tokens.erase(tokens.begin());
      node = new MultiplicationNode(node, parseUnaryExpression(tokens));
    } else if (val == "/") {
      tokens.erase(tokens.begin());
      node = new DivisionNode(node, parseUnaryExpression(tokens));
    } else if (val == "%") {
      tokens.erase(tokens.begin());
      node = new RemainderNode(node, parseUnaryExpression(tokens));
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

Node *parseStatement(std::vector<Token> &tokens) {
  if (tokens[0].value == "var") {
    // ...
  }
  if (tokens[0].value == "while") {
    Node *condition = parseExpression(tokens);
    if (!tokens.size()) {
      std::cerr << "Error: Unexpected end of file\n";
      exit(1);
    }
    except(tokens[0], ":");
    tokens.erase(tokens.begin());
    Node *body = parseStatement(tokens);
  }
  if (tokens[0].value == "if") {
    // ...
  }
  if (tokens[0].value == "break") {
    if (!tokens.size()) {
      std::cerr << "Error: Unexpected end of file\n";
      exit(1);
    }
    except(tokens[1], ";");
  }
  if (tokens[0].value == "continue") {
    if (!tokens.size() < 1) {
      std::cerr << "Error: Unexpected end of file\n";
      exit(1);
    }
    except(tokens[1], ";");
  }
  if (tokens[0].value == "return") {
    // ...
  }
  if (tokens[0].value == "fn") {
    // ...
  }
  parseExpression(tokens);
}


