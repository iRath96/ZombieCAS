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

#include "Term.h"
#include "Function.h"

namespace Zombie {
  namespace Terms {
    class Invocation : public Term {
    public:
      Invocation(const Definitions::Function *function, TermVector arguments) : function(function), arguments(arguments) {};
      
      const Definitions::Function *function;
      TermVector arguments;
      
      const std::string latex() const {
        std::ostringstream os;
        os << function->name << "(";
        for(auto it = arguments.begin(); it != arguments.end(); ++it) {
          if(it != arguments.begin()) os << ", ";
          os << (*it)->latex();
        } os << ")";
        return os.str();
      };
    };
  }
}

#endif /* defined(__ZombieCAS__Invocation__) */