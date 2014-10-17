//
//  OpAdd.cpp
//  ZombieCAS
//
//  Created by Alexander Rath on 11.10.14.
//  Copyright (c) 2014 Alexander Rath. All rights reserved.
//

#include "OpAdd.h"

#include "OpMultiply.h"
#include "Constant.h"

using namespace Zombie::Terms;

void OpAdd::operator+=(TermSharedPtr other) {
  operands.push_back(other);
}

void OpAdd::operator-=(TermSharedPtr other) {
  operands.push_back(
    TermSharedPtr(
      new OpMultiply(TermVectorShared {
        TermSharedPtr(new Constant(-1)),
        other
      })
    )
  );
}

TermSharedPtr OpAdd::deriveUntidy(const Variable &var) const {
  OpAdd *result = new OpAdd();
  for(auto it = operands.begin(); it != operands.end(); ++it)
    *result += (*it)->deriveUntidy(var);
  return TermSharedPtr(result);
}

TermSharedPtr OpAdd::simplify(TermSharedPtr &self) {
  for(size_t i = 0, j = operands.size(); i < j; ++i)
    operands[i] = operands[i]->simplify(operands[i]);
  return self->tidy(self);
}