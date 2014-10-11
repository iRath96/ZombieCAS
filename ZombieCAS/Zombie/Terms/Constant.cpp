//
//  Constant.cpp
//  ZombieCAS
//
//  Created by Alexander Rath on 11.10.14.
//  Copyright (c) 2014 Alexander Rath. All rights reserved.
//

#include "Constant.h"

using namespace Zombie::Terms;

Constant Constant::orderOf(const Term *t) const {
  if(dynamic_cast<const Constant *>(t)) return 1;
  return Constant(0);
}