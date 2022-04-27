#include <iostream>
#include <vector>
#include <map>

struct Value {
  virtual ~Value() {}
};

struct Number : Value{
  double val;
};

struct String : Value {
  std::string val;
  String(std::string val) : val(val);
};

struct Dictionary : Value {
  std::map<std::string, Value*> val;
};

struct Function : Value {
  ExpressionNode *node;
  Function(StatementNode *node) : node(node);
};

struct Context {
  std::vector<std::map<std::string, Value*>> scopes;
  std::map<std::string, Function*> funcs;
};

struct Empty : Value {};

