//
//  OpAdd.h
//  ZombieCAS
//
//  Created by Alexander Rath on 11.10.14.
//  Copyright (c) 2014 Alexander Rath. All rights reserved.
//

#ifndef __ZombieCAS__OpAdd__
#define __ZombieCAS__OpAdd__

#include <iostream>
#include <sstream>
#include "Operation.h"

namespace Zombie {
  namespace Terms {
    class OpAdd : public Operation {
    public:
      OpAdd() {}
      
      template<class T>
      OpAdd(T o) : Operation(o) {}
      
      void operator +=(TermSharedPtr other);
      void operator -=(TermSharedPtr other);
      
      /*
       Constant orderOf(const Term *t) const {
       if(operands.size() == 0) return 0;
       
       #define MAX(a,b) (a) > (b) ? (a) : (b)
       auto it = operands.begin();
       Constant max = (*it)->orderOf(t);
       while(max > 0 && it != operands.end()) max = MAX(max, (*++it)->orderOf(t));
       #undef MAX
       
       return max;
       }*/
      
      virtual TermSharedPtr simplify(TermSharedPtr &);
      virtual TermSharedPtr deriveUntidy(const Variable &) const;
      
      Number calculate(const Arguments &a) const {
        Number result = 0;
        for(auto it = operands.begin(); it != operands.end(); ++it) result += (*it)->calculate(a);
        return result;
      }
      
      const short sign() const {
        for(auto it = operands.begin(); it != operands.end(); ++it) if((*it)->sign() == +1) return +1;
        return -1;
      }
      
      const std::string toString() const {
        std::ostringstream os;
        for(auto it = operands.begin(); it != operands.end(); ++it) {
          if(it != operands.begin()) os << " + ";
          if(dynamic_cast<const Operation *>(it->get()))
            os << "(" << (*it)->toString() << ")";
          else
            os << (*it)->toString();
        } return os.str();
      }
      
      const std::string latex(const latex_ctx_t &ctx) const {
        std::ostringstream osP, osN;
        
        if(ctx.parentalPrecedence > kLP_ADD) osP << "(";
        
        for(auto it = operands.begin(); it != operands.end(); ++it) {
          if(it != operands.begin()) osP << "+";
          osP << (*it)->latex((latex_ctx_t){ kLP_ADD });
        }
        
        if(ctx.parentalPrecedence > kLP_ADD) osP << ")";
        return osP.str();
      }
    };
  }
}

#endif /* defined(__ZombieCAS__OpAdd__) */
