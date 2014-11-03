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
      Operation(TermVector operands) {
        // Attention: This will take ownership, so please
        // pass only pointers to heap objects that you may
        // not delete yourself.
        
        for(auto it = operands.begin(); it != operands.end(); ++it)
          this->operands.push_back(std::shared_ptr<Term>(*it));
      }
      
      Operation(TermVectorShared operands) : operands(operands) {}
      
    public:
      virtual TermSharedPtr tidy(TermSharedPtr &self);
      virtual TermSharedPtr deriveUntidy(const Variable &) const = 0;
      
      virtual Number calculate(const Arguments &) const = 0;
      
      virtual const short sign() const = 0;
      virtual const std::string toString() const = 0;
      virtual const std::string latex(const latex_ctx_t &) const = 0;
      
      TermVectorShared operands;
      
      virtual bool operator ==(const Term &other) const;
    };
  }
}

#endif /* defined(__ZombieCAS__Operation__) */