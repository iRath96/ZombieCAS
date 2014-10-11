//
//  OpPow.cpp
//  ZombieCAS
//
//  Created by Alexander Rath on 11.10.14.
//  Copyright (c) 2014 Alexander Rath. All rights reserved.
//

#include "OpPow.h"
#include "Constant.h"

using namespace Zombie::Terms;

void OpPow::tidy() {
  if(operands.size() < 2) return;
  
  bool baseConst = dynamic_cast<Constant *>(operands[0].get()) != NULL;
  bool expConst  = dynamic_cast<Constant *>(operands[1].get()) != NULL;
  
       if(baseConst && *(Constant *)(operands[0].get()) == Constant(0)) operands.pop_back();
  else if(baseConst && *(Constant *)(operands[0].get()) == Constant(1)) operands.pop_back();
  else if(expConst  && *(Constant *)(operands[1].get()) == Constant(1)) operands.pop_back();
  else if(expConst  && *(Constant *)(operands[1].get()) == Constant(0)) {
    operands.clear();
    operands.push_back(std::unique_ptr<Term>(new Constant(1)));
  }
}