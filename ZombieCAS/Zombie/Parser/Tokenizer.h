//
//  Tokenizer.h
//  ZombieCAS
//
//  Created by Alexander Rath on 11.10.14.
//  Copyright (c) 2014 Alexander Rath. All rights reserved.
//

#ifndef __ZombieCAS__Tokenizer__
#define __ZombieCAS__Tokenizer__

#include "Token.h"
#include <iostream>
#include <vector>

namespace Zombie {
  namespace Parser {
    class Tokenizer {
    public:
      std::vector<Token *> tokens;
      Tokenizer(std::string);
      void dump() const;
    };
  }
}

#endif /* defined(__ZombieCAS__Tokenizer__) */