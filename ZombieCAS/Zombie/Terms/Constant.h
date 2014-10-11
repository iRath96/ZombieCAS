//
//  Constant.h
//  ZombieCAS
//
//  Created by Alexander Rath on 11.10.14.
//  Copyright (c) 2014 Alexander Rath. All rights reserved.
//

#ifndef __ZombieCAS__Constant__
#define __ZombieCAS__Constant__

#include <iostream>
#include <sstream>

#include "Term.h"

namespace Zombie {
  namespace Terms {
    class Constant : public Term {
    public:
      double n = 1.f;
      unsigned long d = 1;
      
      Constant(double v) : n(v) {}
      
      Constant orderOf(const Term *) const;
      
      const std::string latex() const {
        std::ostringstream os;
        os << n;
        if(d != 1) os << "/" << d;
        return os.str();
      };
    };
  }
}

#endif /* defined(__ZombieCAS__Constant__) */