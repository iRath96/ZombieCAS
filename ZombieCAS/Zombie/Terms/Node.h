//
//  Node.h
//  ZombieCAS
//
//  Created by Alexander Rath on 18.10.14.
//  Copyright (c) 2014 Alexander Rath. All rights reserved.
//

#ifndef __ZombieCAS__Node__
#define __ZombieCAS__Node__

#include <iostream>

#include "Term.h"
#include "Function.h"

namespace Zombie {
  namespace Terms {
    class Node {
    public:
      TermSharedPtr term;
      
      Node() {}
      Node(TermSharedPtr term) : term(term) {}
      
      void operator =(TermSharedPtr &t) { term = t; }
      
      void expand(bool tidy = true);
      void simplify(bool tidy = true);
      void tidy();
      
      Node derive(const Variable &var) { return Node(term->derive(var)); }
      std::string latex() { return term->latex(); }
    };
  }
}

#endif /* defined(__ZombieCAS__Node__) */