//
//  Operation.h
//  ZombieCAS
//
//  Created by Alexander Rath on 11.10.14.
//  Copyright (c) 2014 Alexander Rath. All rights reserved.
//

#ifndef __ZombieCAS__Operation__
#define __ZombieCAS__Operation__

#include <iostream>
#include <vector>

#include "Term.h"

namespace Zombie {
  namespace Terms {
    class Operation : public Term {
    protected:
      Operation() {}
      Operation(TermVector operands) : operands(operands) {};
      
    public:
      virtual const std::string latex() const = 0;
      TermVector operands;
    };
  }
}

#endif /* defined(__ZombieCAS__Operation__) */