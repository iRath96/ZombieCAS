//
//  Variable.cpp
//  ZombieCAS
//
//  Created by Alexander Rath on 11.10.14.
//  Copyright (c) 2014 Alexander Rath. All rights reserved.
//

#include "Variable.h"

#include "Constant.h"

using namespace Zombie::Terms;

TermSharedPtr Variable::deriveUntidy(const Variable &var) const {
  return TermSharedPtr(new Constant(*this == var ? 1 : 0));
}