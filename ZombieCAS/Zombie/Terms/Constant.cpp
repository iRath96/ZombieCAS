//
//  Constant.cpp
//  ZombieCAS
//
//  Created by Alexander Rath on 11.10.14.
//  Copyright (c) 2014 Alexander Rath. All rights reserved.
//

#include "Constant.h"

using namespace Zombie::Terms;

long gcd(long m, long n) {
  if(n == 0) return m;
  else return gcd(n, m % n);
}

TermSharedPtr Constant::tidy(TermSharedPtr &self) {
  bool numericNominator = n == (long)n;
  if(!numericNominator && n * 10e8 == (long)(n * 10e8)) {
    n *= 10e8;
    d *= 10e8;
    numericNominator = true;
  }
  
  if(numericNominator) {
    long div = gcd(n, d);
    if(div < 0) div = -div;
    n /= div;
    d /= div;
  } return self;
}

Constant Constant::orderOf(const Term *t) const {
  if(dynamic_cast<const Constant *>(t)) return 1;
  return Constant(0);
}