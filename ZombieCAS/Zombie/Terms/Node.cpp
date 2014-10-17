//
//  Node.cpp
//  ZombieCAS
//
//  Created by Alexander Rath on 18.10.14.
//  Copyright (c) 2014 Alexander Rath. All rights reserved.
//

#include "Node.h"

using namespace Zombie::Terms;

void Node::tidy() { term = term->tidy(term); }
void Node::simplify(bool tidy) {
  term = term->simplify(term);
  if(tidy) term = term->tidy(term);
}

void Node::expand(bool tidy) {
  term = term->expand(term);
  if(tidy) term = term->tidy(term);
}