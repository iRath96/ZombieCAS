//
//  Invocation.cpp
//  ZombieCAS
//
//  Created by Alexander Rath on 11.10.14.
//  Copyright (c) 2014 Alexander Rath. All rights reserved.
//

#include "Invocation.h"

using namespace Zombie::Terms;

void Invocation::tidy() {
  for(auto it = arguments.begin(); it != arguments.end(); ++it)
    (*it)->tidy();
}