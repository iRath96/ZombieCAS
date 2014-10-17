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
#include "Function.h"
#include "Invocation.h"

using namespace Zombie::Terms;

TermSharedPtr OpPow::tidy(TermSharedPtr &self) {
  if(operands.size() < 2) return operands[0];
  
  operands[0] = operands[0]->tidy(operands[0]);
  operands[1] = operands[1]->tidy(operands[1]);
  
  bool baseConst = dynamic_cast<Constant *>(operands[0].get()) != NULL;
  bool expConst  = dynamic_cast<Constant *>(operands[1].get()) != NULL;
  
       if(baseConst && *(Constant *)(operands[0].get()) == 0) operands.pop_back();
  else if(baseConst && *(Constant *)(operands[0].get()) == 1) operands.pop_back();
  else if(expConst  && *(Constant *)(operands[1].get()) == 1) operands.pop_back();
  else if(expConst  && *(Constant *)(operands[1].get()) == 0)
    return TermSharedPtr(new Constant(1));
  else if(dynamic_cast<OpPow *>(operands[0].get())) {
    // Our base is a power, too.
    // Multiply both exponents and use the base of our base.
    
    OpPow *base = (OpPow *)operands[0].get();
    
    TermSharedPtr exp = base->operands.size() == 1 ?
      operands[1] :
      TermSharedPtr(new OpMultiply(TermVectorShared { base->operands[1], operands[1] }));
    exp = (*exp).tidy(exp);
    
    operands[1] = std::shared_ptr<Term>(exp);
    operands[0] = std::move(base->operands[0]);
    
    return tidy(self); // TODO:2014-10-11:alex:Is this required? Could this yield infinite recursion?
  }
  
  return self;
}

TermSharedPtr OpPow::deriveUntidy(const Variable &var) const {
  // Think of this as
  //   [ e ^ (ln(base) * exp) ]'
  // = [ ln(base) * exp ]' * e ^ (ln(base) * exp)
  // = [ ln(base) * exp ]' * base ^ exp
  
  TermSharedPtr mul = TermSharedPtr(new OpMultiply(TermVectorShared {
    TermSharedPtr(new Invocation(new Definitions::Function("ln"), TermVectorShared { operands[0] })),
    operands[1]
  }));
  
  mul = TermSharedPtr(new OpMultiply(TermVectorShared {
    mul->derive(var),
    TermSharedPtr(new OpPow(TermVectorShared { operands[0], operands[1] }))
  }));
  
  return TermSharedPtr(mul);
}