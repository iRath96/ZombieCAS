//
//  OpPow.cpp
//  ZombieCAS
//
//  Created by Alexander Rath on 11.10.14.
//  Copyright (c) 2014 Alexander Rath. All rights reserved.
//

#include "OpPow.h"

#include "OpMultiply.h"
#include "Constant.h"

using namespace Zombie::Terms;

void OpPow::tidy() {
  if(operands.size() < 2) return;
  
  operands[0]->tidy();
  operands[1]->tidy();
  
  bool baseConst = dynamic_cast<Constant *>(operands[0].get()) != NULL;
  bool expConst  = dynamic_cast<Constant *>(operands[1].get()) != NULL;
  
       if(baseConst && *(Constant *)(operands[0].get()) == 0) operands.pop_back();
  else if(baseConst && *(Constant *)(operands[0].get()) == 1) operands.pop_back();
  else if(expConst  && *(Constant *)(operands[1].get()) == 1) operands.pop_back();
  else if(expConst  && *(Constant *)(operands[1].get()) == 0) {
    operands.clear();
    operands.push_back(std::unique_ptr<Term>(new Constant(1)));
  } else if(dynamic_cast<OpPow *>(operands[0].get())) {
    // Our base is a power, too.
    // Multiply both exponents and use the base of our base.
    
    OpPow *base = (OpPow *)operands[0].release();
    
    Term *exp = base->operands.size() == 1 ?
      operands[1].release() :
      new OpMultiply(TermVector { base->operands[1].release(), operands[1].release() });
    (*exp).tidy();
    
    operands[1] = std::unique_ptr<Term>(exp);
    operands[0] = std::move(base->operands[0]);
    
    tidy(); // TODO:2014-10-11:alex:Is this required? Could this yield infinite recursion?
  }
}