//
//  Invocation.cpp
//  ZombieCAS
//
//  Created by Alexander Rath on 11.10.14.
//  Copyright (c) 2014 Alexander Rath. All rights reserved.
//

#include "Invocation.h"

using namespace Zombie::Terms;

TermSharedPtr Invocation::deriveUntidy(const Variable &var) const {
  if(function->name == "ln") {
    TermSharedPtr derivate = arguments[0]->derive(var);
    TermSharedPtr divisor = arguments[0];
    return Term::divide(derivate, divisor);
  }
  
  TermSharedPtr pOne = TermSharedPtr(new Constant(+1));
  TermSharedPtr mOne = TermSharedPtr(new Constant(-1));
  
  if(function->name == "sin" || function->name == "cos") {
    // TODO:2014-11-05:alex:This leaks memory.
    Definitions::Function *other = new Definitions::Function(function->name == "sin" ? "cos" : "sin");
    TermSharedPtr derivate = arguments[0]->derive(var);
    TermSharedPtr func = TermSharedPtr(new Invocation(other, TermVectorShared { arguments[0] }));
    TermSharedPtr product = Term::multiply(derivate, func);
    return function->name == "cos" ? Term::multiply(mOne, product) : product;
  }
  
  if(function->name == "arcsin" || function->name == "arccos") {
    TermSharedPtr derivate = arguments[0]->derive(var);
    TermSharedPtr m12 = TermSharedPtr(new Constant(-1, 2));
    TermSharedPtr pTwo = TermSharedPtr(new Constant(2));
    TermSharedPtr arg = arguments[0];
    TermSharedPtr a = Term::pow(arg, pTwo);
    TermSharedPtr b = Term::subtract(pOne, a);
    TermSharedPtr c = Term::pow(b, m12);
    if(function->name == "arccos") c = Term::multiply(mOne, c);
    return Term::multiply(derivate, c);
  }
  
  if(function->name == "tan") {
    Definitions::Function *cos = new Definitions::Function("cos");
    TermSharedPtr derivate = arguments[0]->derive(var);
    TermSharedPtr mTwo = TermSharedPtr(new Constant(-2));
    TermSharedPtr func = TermSharedPtr(new Invocation(cos, TermVectorShared { arguments[0] }));
    TermSharedPtr result = Term::pow(func, mTwo);
    return Term::multiply(derivate, result);
  }
  
  std::cout << "Ouch! Cannot derive function " << function->name << "!" << std::endl;
  return TermSharedPtr(new Constant(0));
}

TermSharedPtr Invocation::tidy(TermSharedPtr &self) {
  for(int i = 0; i < arguments.size(); ++i)
    arguments[i] = arguments[i]->tidy(arguments[i]);
  return self;
}