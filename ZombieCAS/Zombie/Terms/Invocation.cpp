//
//  Invocation.cpp
//  ZombieCAS
//
//  Created by Alexander Rath on 11.10.14.
//  Copyright (c) 2014 Alexander Rath. All rights reserved.
//

#include "Invocation.h"

using namespace Zombie::Terms;

TermSharedPtr Invocation::tidy(TermSharedPtr &self) {
  for(int i = 0; i < arguments.size(); ++i)
    arguments[i] = arguments[i]->tidy(arguments[i]);
  return TermSharedPtr(this);
}