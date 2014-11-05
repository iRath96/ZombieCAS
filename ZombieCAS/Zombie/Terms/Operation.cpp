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
    
    if(typeid(*tidied.get()) == typeid(*this)) {
      Operation *op = (Operation *)tidied.get();
      for(auto it2 = op->operands.begin(); it2 != op->operands.end(); ++it2)
        newOperands.push_back(*it2);
    } else
      newOperands.push_back(tidied);
  }
  
  operands.clear();
  
  bool isMultiply = !!dynamic_cast<OpMultiply *>(this);
  int neutral = isMultiply ? 1 : 0;
  
  TermVectorShared constants;
  
  for(auto it = newOperands.begin(); it != newOperands.end(); ++it) {
    if(dynamic_cast<Constant *>(it->get())) {
      if(isMultiply && *(Constant *)it->get() == 0) {
        TermSharedPtr zero = *it;
        operands.clear();
        return zero;
      } else {
        constants.push_back(*it);
        continue;
      }
    }
    
    operands.push_back(*it);
  }
  
  // This comment serves no purpose.
  
  Constant *result = new Constant(neutral);
  for(auto it = constants.begin(); it != constants.end(); ++it)
    if(isMultiply) *result *= *(Constant *)it->get();
    else *result += *(Constant *)it->get();
  
  TermSharedPtr constResult = TermSharedPtr(result);
  constResult = constResult->tidy(constResult);
  
  if(*result != neutral) operands.push_back(constResult);
  
  if(operands.size() == 0) return TermSharedPtr(new Constant(neutral));
  if(operands.size() == 1) return operands[0];
  
  return self;
}

bool Operation::operator ==(const Term &other) const {
  if(typeid(*this) != typeid(other)) return false;
  if(operands.size() != ((Operation *)&other)->operands.size()) return false;
  
  TermVectorShared op = ((Operation *)&other)->operands;
  for(auto it = operands.begin(); it != operands.end(); ++it) {
    for(auto it2 = op.begin(); it2 != op.end(); ++it2)
      if(*(it->get()) == *(it2->get())) {
        op.erase(it2);
        goto findNext;
      }
    return false;
  findNext:
    continue;
  }
  
  return true;
}