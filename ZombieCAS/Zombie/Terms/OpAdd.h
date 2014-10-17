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
      
      const std::string latex() const {
        std::ostringstream os;
        for(auto it = operands.begin(); it != operands.end(); ++it) {
          if(it != operands.begin()) os << " + ";
          if(dynamic_cast<const Operation *>(it->get()))
            os << "(" << (*it)->latex() << ")";
          else
            os << (*it)->latex();
        } return os.str();
      };
    };
  }
}

#endif /* defined(__ZombieCAS__OpAdd__) */
