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

void Operation::tidy() {
  TermVector newOperands;
  for(auto it = operands.begin(); it != operands.end(); ++it) {
    (*it)->tidy();
    if(typeid(*it->get()) == typeid(*this) ||
       (dynamic_cast<Operation *>(it->get()) && ((Operation *)it->get())->operands.size() < 2)) {
      Operation *op = (Operation *)it->get();
      for(auto it2 = op->operands.begin(); it2 != op->operands.end(); ++it2)
        newOperands.push_back(it2->release());
    } else
      newOperands.push_back(it->release());
  }
  
  operands.clear();
  
  int neutral = dynamic_cast<OpMultiply *>(this) ? 1 : 0;
  for(auto it = newOperands.begin(); it != newOperands.end(); ++it)
    if(!dynamic_cast<Constant *>(*it) || *(Constant *)*it != neutral)
      operands.push_back(std::unique_ptr<Term>(*it));
}

bool Operation::operator ==(const Term &other) const {
  if(typeid(*this) != typeid(other)) return false;
  return true;
}