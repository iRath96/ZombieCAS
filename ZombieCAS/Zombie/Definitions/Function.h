//
//  Function.h
//  ZombieCAS
//
//  Created by Alexander Rath on 11.10.14.
//  Copyright (c) 2014 Alexander Rath. All rights reserved.
//

#ifndef __ZombieCAS__Function__
#define __ZombieCAS__Function__

#include <iostream>
#include <string>

namespace Zombie {
  namespace Definitions {
    class Function {
    public:
      operator Function *() { return this; };
      
      Function(std::string name) : name(name) {}
      std::string name;
    };
  }
}

#endif /* defined(__ZombieCAS__Function__) */