#ifndef __ABNODE_H__
#define __ABNODE_H__

#include "main.hpp"


struct Value;

struct ExpressionNode {
  virtual Value *eval() = 0;
  virtual ~ExpressionNode() {}
};

struct StatementNode {
  virtual void eval() = 0;
  virtual ~StatementNode() {}
};

StatementNode *parseStatement(std::vector<Token> &tokens);

#endif /* __ABNODE_H__ */
