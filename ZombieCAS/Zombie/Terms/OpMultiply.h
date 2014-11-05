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
#include "Constant.h"

namespace Zombie {
  namespace Terms {
    class OpMultiply : public Operation {
    public:
      OpMultiply() {}
      
      template<class T>
      OpMultiply(T o) : Operation(o) {}
      
      OpMultiply &operator *=(TermSharedPtr other) {
        /*if(dynamic_cast<const Constant *>(other)) // We are being multiplied with a zero, let's return zero then.
         if(((Constant *)other)->n == 0.f)
         return *new Constant(0);*/
        
        operands.push_back(other);
        return *this;
      };
      
      /*
       int orderOf(const Term *t) {
       int order = 0;
       for(auto it = operands.begin(); it != operands.end(); ++it) order += (*it)->orderOf(t);
       return order;
       }*/
      
      virtual TermSharedPtr simplify(TermSharedPtr &);
      virtual TermSharedPtr expand(TermSharedPtr &);
      virtual TermSharedPtr deriveUntidy(const Variable &) const;
      
      Number calculate(const Arguments &a) const {
        Number result = 1;
        for(auto it = operands.begin(); it != operands.end(); ++it) result *= (*it)->calculate(a);
        return result;
      }
      
      const short sign() const {
        short sign = +1;
        for(auto it = operands.begin(); it != operands.end(); ++it) sign *= (*it)->sign();
        return sign;
      }
      
      const std::string toString() const {
        std::ostringstream os;
        for(auto it = operands.begin(); it != operands.end(); ++it) {
          if(it != operands.begin()) os << " * ";
          if(dynamic_cast<const Operation *>(it->get()))
            os << "(" << (*it)->toString() << ")";
          else
            os << (*it)->toString();
        }
        
        return os.str();
      }
      
      const std::string latex(const latex_ctx_t &ctx) const;
    };
  }
}

#endif /* defined(__ZombieCAS__OpMultiply__) */