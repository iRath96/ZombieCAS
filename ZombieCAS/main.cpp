//
//  main.cpp
//  ZombieCAS
//
//  Created by Alexander Rath on 09.10.14.
//  Copyright (c) 2014 Alexander Rath. All rights reserved.
//

#include <iostream>
#include <vector>
#include <sstream>
#include <string>

class OpAdd;
class OpMultiply;
class OpPow;
class Constant;

class Term {
public:
  operator Term *() { return this; };
  
#define def_op(op, name, klass) \
  inline virtual klass & name(Term *t); \
  inline virtual klass & name(double v); \
  klass & operator op(Term *other) { return name(other); } \
//template<class T> Term &operator op##=(T other) { std::cout << "op" #op "=" << std::endl; return *this = name(other); }
  
  def_op(*, multiply, OpMultiply)
  def_op(/, divide,   OpMultiply)
  def_op(+, add,      OpAdd)
  def_op(-, subtract, OpAdd)
  def_op(^, pow,      OpPow)
#undef def_op
  
  Term &operator=(const Term *rhs) {
    std::cout << "Magic" << std::endl;
    if(this != rhs) {
      // TODO:2014-10-09:alex:Do some magic here.
    } return *this;
  };
  
  // TODO:2014-10-09:alex:Copy constructor.
  // TODO:2014-10-09:alex:Expand and simplify.
  
//virtual Constant orderOf(const Term *) const = 0;
  virtual const std::string latex() const = 0;
};

class Constant : public Term {
public:
  double n = 1.f;
  unsigned long d = 1;
  
  Constant(double v) : n(v) {}
  
  Constant orderOf(const Term *) const;
  
  const std::string latex() const {
    std::ostringstream os;
    os << n;
    if(d != 1) os << "/" << d;
    return os.str();
  };
};

Constant Constant::orderOf(const Term *t) const {
  if(dynamic_cast<const Constant *>(t)) return 1;
  return Constant(0);
}

#define Terms std::vector<const Term *>
class Operation : public Term {
protected:
  Operation() {}
  Operation(Terms operands) : operands(operands) {};
  
public:
  virtual const std::string latex() const = 0;
  Terms operands;
};

class OpAdd : public Operation {
public:
  OpAdd() {}
  OpAdd(Terms o) : Operation(o) {}
  
  OpAdd &operator +=(const Term *other);
  OpAdd &operator -=(const Term *other);
  
  /*
  Constant orderOf(const Term *t) const {
    if(operands.size() == 0) return 0;
    
#define MAX(a,b) (a) > (b) ? (a) : (b)
    auto it = operands.begin();
    Constant max = (*it)->orderOf(t);
    while(max > 0 && it != operands.end()) max = MAX(max, (*++it)->orderOf(t));
#undef MAX
    
    return max;
  }*/
  
  const std::string latex() const {
    std::ostringstream os;
    for(auto it = operands.begin(); it != operands.end(); ++it) {
      if(it != operands.begin()) os << " + ";
      if(dynamic_cast<const Operation *>(*it))
        os << "(" << (*it)->latex() << ")";
      else
        os << (*it)->latex();
    } return os.str();
  };
};

class OpMultiply : public Operation {
public:
  OpMultiply() {}
  OpMultiply(Terms o) : Operation(o) {}
  
  OpMultiply &operator *=(const Term *other) {
    /*if(dynamic_cast<const Constant *>(other)) // We are being multiplied with a zero, let's return zero then.
      if(((Constant *)other)->n == 0.f)
        return *new Constant(0);*/
    
    operands.push_back(other);
    return *this;
  };
  
  /*
  int orderOf(const Term *t) {
    int order = 0;
    for(auto it = operands.begin(); it != operands.end(); ++it) order += (*it)->orderOf(t);
    return order;
  }*/
  
  const std::string latex() const {
    std::ostringstream os;
    for(auto it = operands.begin(); it != operands.end(); ++it) {
      if(it != operands.begin()) os << " * ";
      if(dynamic_cast<const Operation *>(*it))
        os << "(" << (*it)->latex() << ")";
      else
        os << (*it)->latex();
    } return os.str();
  };
};

class OpPow : public Operation {
public:
  OpPow() {}
  OpPow(Terms o) : Operation(o) {
    if(o.size() != 2) std::cout << "What?" << std::endl;
  }
  
  /*
  int orderOf(const Term *t) {
    int order = 0;
    for(auto it = operands.begin(); it != operands.end(); ++it) order += (*it)->orderOf(t);
    return order;
  }*/
  
  const std::string latex() const {
    std::ostringstream os;
    for(auto it = operands.begin(); it != operands.end(); ++it) {
      if(it != operands.begin()) os << " ^ ";
      os << (*it)->latex();
    } return os.str();
  };
};

class Variable : public Term {
public:
  std::string name;
  Variable(std::string name) : name(name) {}
  
  const std::string latex() const { return name; };
};

class Function {
public:
  operator Function *() { return this; };
  
  Function(std::string name) : name(name) {}
  std::string name;
};

class Invocation : public Term {
public:
  Invocation(const Function *function, Terms arguments) : function(function), arguments(arguments) {};
  
  const Function *function;
  Terms arguments;
  
  const std::string latex() const {
    std::ostringstream os;
    os << function->name << "(";
    for(auto it = arguments.begin(); it != arguments.end(); ++it) {
      if(it != arguments.begin()) os << ", ";
      os << (*it)->latex();
    } os << ")";
    return os.str();
  };
};

#define def_op(name, klass) \
  klass &Term::name(double v) { return name(new Constant(v)); } \
  klass &Term::name(Term *other)
// Madness.
def_op(multiply, OpMultiply) { return *new OpMultiply(Terms { this, other }); };
def_op(divide,   OpMultiply) { return *new OpMultiply(Terms { this, new OpPow(Terms { other, new Constant(-1) }) }); };
def_op(add,      OpAdd)      { return *new OpAdd(Terms { this, other }); };
def_op(subtract, OpAdd)      { return *new OpAdd(Terms { this, new OpMultiply(Terms { new Constant(-1), other }) }); };
def_op(pow,      OpPow)      { return *new OpPow(Terms { this, other }); };
#undef def_op

OpAdd &OpAdd::operator+=(const Term *other) {
  operands.push_back(other);
  return *this;
};

OpAdd &OpAdd::operator-=(const Term *other) {
  operands.push_back(new OpMultiply(Terms { new Constant(-1), other }));
  return *this;
};

#include "Tokenizer.h"
#include "Parser.h"
#include "Token.h"

using namespace Zombie;

#define _bind(n) n(#n)
int main(int argc, const char *argv[]) {
  Parser::Tokenizer t("sin((a+b)x,z)^2");
  Parser::Parser p(t.tokens);
  
  t.dump();
  p.dump();
  
  Constant pi(3.14159f);
  Variable _bind(x), _bind(y);
  
  Function _bind(sin);
  Invocation inv(sin, Terms { pi });
  
  OpAdd sum;
  sum += x * pi;
  sum -= x;
  sum += x * inv;
  
  std::cout << sum.latex() << std::endl;
//std::cout << sum.orderOf(x) << std::endl;
  
  return 0;
}