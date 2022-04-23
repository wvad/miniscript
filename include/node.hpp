#include <map>
#include "abnode.hpp"

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

struct ObjectLiteralNode : ExpressionNode {
  std::map<std::string, ExpressionNode*> members;
  ObjectLiteralNode(std::map<std::string, ExpressionNode*> members) : members(members) {}
};

struct WhileNode : StatememtNode {
  ExpressionNode *condition;
  StatememtNode* body;
  WhileNode(ExpressionNode *condition, StatememtNode* body) : condition(condition), body(body) {}
};

struct IfNode : StatememtNode {
  ExpressionNode *condition;
  StatememtNode* trueBranch;
  StatememtNode* falseBranch;
  IfNode(ExpressionNode *condition, StatememtNode* trueBranch)
      : condition(condition), trueBranch(trueBranch), falseBranch(nullptr) {}
  IfNode(ExpressionNode *condition, StatememtNode* trueBranch, StatememtNode* falseBranch)
      : condition(condition), trueBranch(trueBranch), falseBranch(falseBranch) {}
};

struct BreakNode : StatememtNode {};

struct ContinueNode : StatememtNode {};

struct ReturnNode : StatememtNode {
  ExpressionNode *value;
  ReturnNode(ExpressionNode *value) : value(value) {}
};

struct VariableDeclarationNode : StatememtNode {
  std::string name;
  ExpressionNode *value;
  VariableDeclarationNode(std::string name, ExpressionNode *value) : name(name), value(value) {}
};

struct FunctionDeclarationNode : StatememtNode {
  std::string name;
  std::vector<std::string> args;
  StatememtNode* body;
  FunctionDeclarationNode(std::string name, std::vector<std::string> args, StatememtNode* body)
      : name(name), args(args), body(body) {}
};

struct ExpressionStatementNode : StatememtNode {
  ExpressionNode *expression;
  ExpressionStatementNode(ExpressionNode *expression) : expression(expression) {}
};

struct BlockNode : StatememtNode {
  std::vector<StatememtNode*> statements;
  BlockNode(std::vector<StatememtNode*> statements) : statements(statements) {}
};
