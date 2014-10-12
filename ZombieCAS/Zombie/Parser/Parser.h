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
#include <vector>
#include <string>

#include "Token.h"
#include "Term.h"

namespace Zombie {
  namespace Parser {
    class Parser {
    public:
      Parser(std::string);
      Parser(const std::vector<Token *> &);
      
      std::vector<const Token *> rpn;
      
      void dump() const;
      
      Terms::Term *buildAST(bool tidy = true) const;
      
    private:
      void buildRPN(const std::vector<Token *> &);
    };
  }
}

#endif /* defined(__ZombieCAS__Parser__) */