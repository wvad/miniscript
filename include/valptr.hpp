#ifndef __VALPTR_H__
#define __VALPTR_H__

struct Value;

struct ValuePointer {
  Value *value;
  ValuePointer() : value(0) {}
  ValuePointer(Value *value) : value(value) {}
  Value *operator->() { return value; }
  virtual ~ValuePointer() {}
};

#endif /* __VALPTR_H__ */
