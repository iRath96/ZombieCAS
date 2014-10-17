//
//  Term.cpp
//  ZombieCAS
//
//  Created by Alexander Rath on 11.10.14.
//  Copyright (c) 2014 Alexander Rath. All rights reserved.
//

#include "Term.h"

#include "OpMultiply.h"
#include "OpAdd.h"
#include "OpPow.h"
#include "Constant.h"

using namespace Zombie::Terms;

#define def_op(name) \
TermSharedPtr Term::name(TermSharedPtr &a, TermSharedPtr &b)
def_op(multiply) {
  return TermSharedPtr(new OpMultiply(TermVectorShared {
    a,
    b
  }));
};

def_op(divide) {
  return TermSharedPtr(new OpMultiply(TermVectorShared {
    a,
    TermSharedPtr(new OpPow(TermVectorShared {
      b,
      (TermSharedPtr)new Constant(-1)
    }))
  }));
};

def_op(add) {
  return TermSharedPtr(new OpAdd(TermVectorShared {
    a,
    b
  }));
};

def_op(subtract) {
  return TermSharedPtr(new OpAdd(TermVectorShared {
    a,
    TermSharedPtr(new OpMultiply(TermVectorShared {
      (TermSharedPtr)new Constant(-1),
      b
    }))
  }));
};

def_op(pow) {
  return TermSharedPtr(new OpPow(TermVectorShared {
    a,
    b
  }));
};

#undef def_op