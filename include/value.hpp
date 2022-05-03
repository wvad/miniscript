#ifndef __VALUE_H__
#define __VALUE_H__

#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include "valptr.hpp"

enum class ValueKind {
  NUMBER,
  STRING,
  DICTIONARY,
  FUNCTION,
  EMPTY
};

struct Value {
  const ValueKind kind;
  Value(ValueKind kind) : kind(kind) {}
  virtual ~Value() {}
};

std::vector<ValuePointer> pointers;

struct Empty : Value {
  Empty() : Value(ValueKind::EMPTY) {}
};

struct Number : Value {
  const double val;
  Number(double val) : val(val), Value(ValueKind::NUMBER) {}
};

struct String : Value {
  const std::string val;
  String(std::string val) : val(val), Value(ValueKind::STRING) {}
};

struct Dictionary : Value {
  ValuePointer operator[](std::string key) {
    return val.find(key) == val.end() ? ValuePointer(new Empty()) : val[key];
  }
  Dictionary() : Value(ValueKind::DICTIONARY) {}
private:
  std::map<std::string, ValuePointer> val;
};

struct StatementNode;

struct Function : Value {
  StatementNode *node;
  Function(StatementNode *node) : node(node), Value(ValueKind::FUNCTION) {}
};

struct Context {
  std::stack<ValuePointer*> scopes;
  std::map<std::string, Function*> funcs;
};

#endif /* __VALUE_H__ */
