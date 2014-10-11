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

OpAdd &OpAdd::operator+=(const Term *other) {
  operands.push_back(other);
  return *this;
};

OpAdd &OpAdd::operator-=(const Term *other) {
  operands.push_back(new OpMultiply(TermVector { new Constant(-1), other }));
  return *this;
};