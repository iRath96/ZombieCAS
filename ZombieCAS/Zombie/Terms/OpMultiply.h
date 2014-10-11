//
//  OpMultiply.h
//  ZombieCAS
//
//  Created by Alexander Rath on 11.10.14.
//  Copyright (c) 2014 Alexander Rath. All rights reserved.
//

#ifndef __ZombieCAS__OpMultiply__
#define __ZombieCAS__OpMultiply__

#include <iostream>
#include <sstream>
#include "Operation.h"

namespace Zombie {
  namespace Terms {
    class OpMultiply : public Operation {
    public:
      OpMultiply() {}
      OpMultiply(TermVector o) : Operation(o) {}
      
      OpMultiply &operator *=(Term *other) {
        /*if(dynamic_cast<const Constant *>(other)) // We are being multiplied with a zero, let's return zero then.
         if(((Constant *)other)->n == 0.f)
         return *new Constant(0);*/
        
        operands.push_back(std::unique_ptr<Term>(other));
        return *this;
      };
      
      /*
       int orderOf(const Term *t) {
       int order = 0;
       for(auto it = operands.begin(); it != operands.end(); ++it) order += (*it)->orderOf(t);
       return order;
       }*/
      
      const std::string latex() const {
        std::ostringstream os;
        for(auto it = operands.begin(); it != operands.end(); ++it) {
          if(it != operands.begin()) os << " * ";
          if(dynamic_cast<const Operation *>(it->get()))
            os << "(" << (*it)->latex() << ")";
          else
            os << (*it)->latex();
        } return os.str();
      };
    };
  }
}

#endif /* defined(__ZombieCAS__OpMultiply__) */