//
//  Parser.h
//  ZombieCAS
//
//  Created by Alexander Rath on 11.10.14.
//  Copyright (c) 2014 Alexander Rath. All rights reserved.
//

#ifndef __ZombieCAS__Parser__
#define __ZombieCAS__Parser__

#include <iostream>
#include "Token.h"
#include <vector>

namespace Zombie {
  namespace Parser {
    class Parser {
    public:
      Parser(std::string str) {}
      Parser(const std::vector<Token *> &tokens);
      
      std::vector<const Token *> rpn;
      
      void dump() const;
    };
  }
}

#endif /* defined(__ZombieCAS__Parser__) */