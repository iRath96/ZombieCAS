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
      const std::string latex() const { return name; };
      
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