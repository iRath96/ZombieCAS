//
//  Variable.h
//  ZombieCAS
//
//  Created by Alexander Rath on 11.10.14.
//  Copyright (c) 2014 Alexander Rath. All rights reserved.
//

#ifndef __ZombieCAS__Variable__
#define __ZombieCAS__Variable__

#include <iostream>
#include "Term.h"

namespace Zombie {
  namespace Terms {
    class Variable : public Term {
    public:
      std::string name;
      Variable(std::string name) : name(name) {}
      
      virtual TermSharedPtr tidy(TermSharedPtr &self) { return self; }
      
      Number calculate(const Arguments &a) const { return a.at(name); }
      
      const short sign() const { return +1; }
      const std::string toString() const { return name; }
      const std::string latex(const latex_ctx_t &ctx) const {
        if(name == "pi") return "\\" + name;
        return name; // TODO:2014-11-03:alex:Verify that this is true.
      }
      
      virtual TermSharedPtr deriveUntidy(const Variable &) const;
      
      bool operator ==(const Term &other) const {
        if(dynamic_cast<const Variable *>(&other))
          return ((const Variable *)&other)->name == name;
        return false;
      }
    };
  }
}

#endif /* defined(__ZombieCAS__Variable__) */