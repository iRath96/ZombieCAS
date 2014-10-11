//
//  Token.cpp
//  ZombieCAS
//
//  Created by Alexander Rath on 11.10.14.
//  Copyright (c) 2014 Alexander Rath. All rights reserved.
//

#include "Token.h"

using namespace Zombie::Parser;

Token::Token(char chr) {
  text = chr;
  rightToLeft = false;
  
  switch(text[0]) {
    case ',': precedence = 1; break;
    case '(': precedence = 7; break;
    case ')': precedence = 1; break;
    case '^': precedence = 4; rightToLeft = true; break;
    case '*': case '/': precedence = 3; break;
    case '+': case '-': precedence = 2; break;
    default: precedence = 0;
  }
}