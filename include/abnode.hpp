#ifndef __ABNODE_H__
#define __ABNODE_H__

struct ExpressionNode {
  virtual Value eval() = 0;
  virtual ~ExpressionNode() {}
};

struct StatememtNode {
  virtual void eval() = 0;
  virtual ~StatementNode {}
};

StatememtNode *parseStatement(std::vector<Token> &tokens);

#endif /* __ABNODE_H__ */
