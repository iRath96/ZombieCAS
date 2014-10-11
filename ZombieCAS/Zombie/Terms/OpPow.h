//
//  OpPow.h
//  ZombieCAS
//
//  Created by Alexander Rath on 11.10.14.
//  Copyright (c) 2014 Alexander Rath. All rights reserved.
//

#ifndef __ZombieCAS__OpPow__
#define __ZombieCAS__OpPow__

#include <iostream>
#include <sstream>
#include "Operation.h"

namespace Zombie {
  namespace Terms {
    class OpPow : public Operation {
    public:
      OpPow() {}
      OpPow(TermVector o) : Operation(o) {
        if(o.size() != 2) std::cout << "What?" << std::endl;
      }
      
      /*
       int orderOf(const Term *t) {
       int order = 0;
       for(auto it = operands.begin(); it != operands.end(); ++it) order += (*it)->orderOf(t);
       return order;
       }*/
      
      const std::string latex() const {
        std::ostringstream os;
        for(auto it = operands.begin(); it != operands.end(); ++it) {
          if(it != operands.begin()) os << " ^ ";
          os << (*it)->latex();
        } return os.str();
      };
    };
  }
}

#endif /* defined(__ZombieCAS__OpPow__) */