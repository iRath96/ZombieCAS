//
//  Operation.cpp
//  ZombieCAS
//
//  Created by Alexander Rath on 11.10.14.
//  Copyright (c) 2014 Alexander Rath. All rights reserved.
//

#include "Operation.h"
#include "Constant.h"
#include "OpMultiply.h"

using namespace Zombie::Terms;

TermSharedPtr Operation::tidy(TermSharedPtr &self) {
  TermVectorShared newOperands;
  for(auto it = operands.begin(); it != operands.end(); ++it) {
    TermSharedPtr tidied = (*it)->tidy(*it);
    newOperands.push_back(tidied);
    /*
    if(typeid(*it->get()) == typeid(*this) ||
       (dynamic_cast<Operation *>(tidied.get()) && ((Operation *)tidied.get())->operands.size() < 2)) {
      Operation *op = (Operation *)it->get();
      for(auto it2 = op->operands.begin(); it2 != op->operands.end(); ++it2)
        newOperands.push_back(*it2);
    } else
      newOperands.push_back(*it);*/
  }
  
  operands.clear();
  
  bool isMultiply = !!dynamic_cast<OpMultiply *>(this);
  int neutral = isMultiply ? 1 : 0;
  
  for(auto it = newOperands.begin(); it != newOperands.end(); ++it) {
    if(dynamic_cast<Constant *>(it->get())) {
      if(isMultiply && *(Constant *)it->get() == 0) {
        TermSharedPtr zero = *it;
        operands.clear();
        return zero;
      } else if(*(Constant *)it->get() == neutral) continue;
    }
    
    operands.push_back(std::shared_ptr<Term>(*it));
  }
  
  if(operands.size() == 0) return TermSharedPtr(new Constant(neutral));
  if(operands.size() == 1) return operands[0];
  
  return self;
}

bool Operation::operator ==(const Term &other) const {
  if(typeid(*this) != typeid(other)) return false;
  return true;
}