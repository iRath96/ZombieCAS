//
//  Invocation.h
//  ZombieCAS
//
//  Created by Alexander Rath on 11.10.14.
//  Copyright (c) 2014 Alexander Rath. All rights reserved.
//

#ifndef __ZombieCAS__Invocation__
#define __ZombieCAS__Invocation__

#include <iostream>
#include <sstream>
#include <math.h>

#include "Term.h"
#include "Constant.h"
#include "Function.h"

namespace Zombie {
  namespace Terms {
    class Invocation : public Term {
    public:
      Invocation(const Definitions::Function *function, TermVector arguments) : function(function) {
        // Attention: This will take ownership, so please
        // pass only pointers to heap objects that you may
        // not delete yourself.
        
        for(auto it = arguments.begin(); it != arguments.end(); ++it)
          this->arguments.push_back(std::shared_ptr<Term>(*it));
      }
      
      Invocation(const Definitions::Function *function, TermVectorShared arguments) : function(function), arguments(arguments) {}
      
      const Definitions::Function *function;
      TermVectorShared arguments;
      
      TermSharedPtr deriveUntidy(const Variable &) const;
      TermSharedPtr tidy(TermSharedPtr &self);
      
      Number calculate(const Arguments &a) const {
        Number arg0 = arguments[0]->calculate(a);
#define native(n, func) if(function->name == n) return func(arg0);
        native("ln", ::log);
        native("sin", ::sin);
        native("cos", ::cos);
        native("tan", ::tan);
        native("arcsin", ::asin);
        native("arccos", ::acos);
        native("arctan", ::atan);
#undef native
        std::cout << "Ouch! Cannot calculate function " << function->name << "!" << std::endl;
        return 0;
      }
      
      const short sign() const { return +1; }
      
      const std::string toString() const {
        std::ostringstream os;
        os << function->name << "(";
        for(auto it = arguments.begin(); it != arguments.end(); ++it) {
          if(it != arguments.begin()) os << ", ";
          os << (*it)->toString();
        } os << ")";
        return os.str();
      }
      
      const std::string latex(const latex_ctx_t &ctx) const {
        std::ostringstream os;
        os << "\\operatorname{" << function->name << "}(";
        for(auto it = arguments.begin(); it != arguments.end(); ++it) {
          if(it != arguments.begin()) os << ", ";
          os << (*it)->latex();
        } os << ")";
        return os.str();
      }
      
      bool operator ==(const Term &other) const {
        if(dynamic_cast<const Invocation *>(&other)) {
          const Invocation *o = (const Invocation *)&other;
          if(*o->function != *function) return false;
          if(o->arguments.size() != arguments.size()) return false;
          for(unsigned long i = 0, j = arguments.size(); i < j; ++i)
            if(*o->arguments[i] != *arguments[i])
              return false;
          return true;
        } return false;
      }
    };
  }
}

#endif /* defined(__ZombieCAS__Invocation__) */