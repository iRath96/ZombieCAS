//
//  main.cpp
//  ZombieCAS
//
//  Created by Alexander Rath on 09.10.14.
//  Copyright (c) 2014 Alexander Rath. All rights reserved.
//

#include <iostream>

#include "Tokenizer.h"
#include "Parser.h"
#include "Token.h"

#include "Constant.h"
#include "Variable.h"
#include "Invocation.h"

#include "OpAdd.h"
#include "OpMultiply.h"

#include "Function.h"

using namespace Zombie;
using namespace Zombie::Terms;
using namespace Zombie::Definitions;

#define _bind(n) n(#n)
int main(int argc, const char *argv[]) {
  Parser::Tokenizer t("sin(x+0,y*1)"); // (a-x)sin(x,y)^-2b x
  Parser::Parser p(t.tokens);
  
  t.dump();
  p.dump();
  
  Term *ast = p.buildAST();
  ast->tidy();
  
  std::cout << ast->latex() << std::endl;
  
  Constant pi(3.14159f);
  Variable _bind(x), _bind(y);
  
  Function _bind(sin);
  Invocation inv(sin, TermVector { new Constant(pi) });
  
  OpAdd sum;
  sum += x * pi;
  sum -= x;
  sum += x * inv;
  
  std::cout << sum.latex() << std::endl;
//std::cout << sum.orderOf(x) << std::endl;
  
  return 0;
}