//
//  OpMultiply.cpp
//  ZombieCAS
//
//  Created by Alexander Rath on 11.10.14.
//  Copyright (c) 2014 Alexander Rath. All rights reserved.
//

#include "OpMultiply.h"

#include "Constant.h"
#include "OpAdd.h"

using namespace Zombie::Terms;

TermSharedPtr OpMultiply::deriveUntidy(const Variable &var) const {
  OpAdd *result = new OpAdd();
  for(auto it = operands.begin(); it != operands.end(); ++it) {
    OpMultiply *summand = new OpMultiply();
    for(auto it2 = operands.begin(); it2 != operands.end(); ++it2) {
      TermSharedPtr blah = *it == *it2 ? (*it2)->deriveUntidy(var) : *it2;;
      *summand *= blah;
    }
    *result += TermSharedPtr(summand);
  } return TermSharedPtr(result);
}