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

#define def_op(name, klass) \
inline klass &Term::name(double v) { return name(new Constant(v)); } \
inline klass &Term::name(Term *other)
// Madness.
def_op(multiply, OpMultiply) { return *new OpMultiply(TermVector { this, other }); };
def_op(divide,   OpMultiply) { return *new OpMultiply(TermVector { this, new OpPow(TermVector { other, new Constant(-1) }) }); };
def_op(add,      OpAdd)      { return *new OpAdd(TermVector { this, other }); };
def_op(subtract, OpAdd)      { return *new OpAdd(TermVector { this, new OpMultiply(TermVector { new Constant(-1), other }) }); };
def_op(pow,      OpPow)      { return *new OpPow(TermVector { this, other }); };
#undef def_op