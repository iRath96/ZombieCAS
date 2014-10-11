//
//  Token.h
//  ZombieCAS
//
//  Created by Alexander Rath on 11.10.14.
//  Copyright (c) 2014 Alexander Rath. All rights reserved.
//

#ifndef __ZombieCAS__Token__
#define __ZombieCAS__Token__

#include <iostream>

namespace Zombie {
  namespace Parser {
    class Token {
    public:
      std::string text;
      
      int precedence;
      bool rightToLeft;
      
      Token(char chr);
      void append(char chr) { text += chr; }
      void invoke() { precedence = 5; }
      
      bool hasValue() const { return precedence == 0 || precedence == 5 || precedence == 6; }
      bool isOperator() const { return precedence != 0; }
      bool isInvocation() const { return precedence == 5; }
      
      bool isLiteral() const { return (text[0] >= '0' && text[0] <= '9') || text[0] == '.'; }
    };
  }
}

#endif /* defined(__ZombieCAS__Token__) */