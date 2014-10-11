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
      OpAdd(TermVector o) : Operation(o) {}
      
      OpAdd &operator +=(const Term *other);
      OpAdd &operator -=(const Term *other);
      
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
      
      const std::string latex() const {
        std::ostringstream os;
        for(auto it = operands.begin(); it != operands.end(); ++it) {
          if(it != operands.begin()) os << " + ";
          if(dynamic_cast<const Operation *>(*it))
            os << "(" << (*it)->latex() << ")";
          else
            os << (*it)->latex();
        } return os.str();
      };
    };
  }
}

#endif /* defined(__ZombieCAS__OpAdd__) */
