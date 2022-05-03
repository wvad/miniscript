#ifndef __ABNODE_H__
#define __ABNODE_H__

#include "valptr.hpp"

struct Context;
struct Token;

struct ExpressionNode {
  virtual ValuePointer eval(Context&) = 0;
  virtual ~ExpressionNode() {}
};

struct StatementNode {
  virtual void eval(Context&) = 0;
  virtual ~StatementNode() {}
};

StatementNode *parseStatement(std::vector<Token> &tokens);

#endif /* __ABNODE_H__ */
