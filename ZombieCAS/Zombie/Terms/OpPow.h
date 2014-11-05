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
#include <math.h>

#include "Operation.h"
#include "Constant.h"

namespace Zombie {
  namespace Terms {
    class OpPow : public Operation {
    public:
      OpPow() {}
      
      template<class T>
      OpPow(T o) : Operation(o) {
        if(o.size() != 2) std::cout << "What?" << std::endl;
      }
      
      /*
       int orderOf(const Term *t) {
       int order = 0;
       for(auto it = operands.begin(); it != operands.end(); ++it) order += (*it)->orderOf(t);
       return order;
       }*/
      
      TermSharedPtr deriveUntidy(const Variable &) const;
      
      TermSharedPtr tidy(TermSharedPtr &);
      TermSharedPtr expand(TermSharedPtr &);
      
      Number calculate(const Arguments &a) const {
        return ::pow(operands[0]->calculate(a), operands[1]->calculate(a));
      }
      
      const short sign() const {
        return +1; // TODO:2014-11-03:alex:Think about this.
      }
      
      const std::string toString() const {
        std::ostringstream os;
        for(auto it = operands.begin(); it != operands.end(); ++it) {
          if(it != operands.begin()) os << " ^ ";
          if(dynamic_cast<Operation *>(it->get()))
            os << "(" << (*it)->toString() << ")";
          else
            os << (*it)->toString();
        } return os.str();
      }
      
      const std::string latex(const latex_ctx_t &ctx) const {
        std::ostringstream os;
        
        bool negateExponent = ctx.negateExponent;
        bool frac = false;
        
        if(ctx.negate) os << "-";
        if(ctx.parentalPrecedence < kLP_MULTIPLY && operands[1]->sign() == -1) {
          os << "\\frac{1}{";
          frac = negateExponent = true;
        }
        
        if(ctx.parentalPrecedence >= kLP_POWER_BASE) os << "(";
        if(negateExponent &&
           dynamic_cast<Constant *>(operands[1].get()) && *(Constant *)(operands[1].get()) == -1) {
          os << operands[0]->latex();
        } else
          for(auto it = operands.begin(); it != operands.end(); ++it) {
            if(it != operands.begin()) os << "^";
            os << "{" << (*it)->latex((latex_ctx_t){
              it == operands.begin() ? kLP_POWER_BASE : kLP_ROOT,
              it != operands.begin() && negateExponent
            }) << "}";
          }
        
        if(ctx.parentalPrecedence >= kLP_POWER_BASE) os << ")";
        if(frac) os << "}";
        
        return os.str();
      }
      
      TermSharedPtr simplify(TermSharedPtr &);
    };
  }
}

#endif /* defined(__ZombieCAS__OpPow__) */