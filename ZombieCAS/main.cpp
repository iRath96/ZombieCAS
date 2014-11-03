//
//  main.cpp
//  ZombieCAS
//
//  Created by Alexander Rath on 09.10.14.
//  Copyright (c) 2014 Alexander Rath. All rights reserved.
//

#include <sys/mman.h>
#include <sys/time.h>
#include <string.h>

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

#include "Node.h"

using namespace Zombie;
using namespace Zombie::Terms;
using namespace Zombie::Definitions;

#define _bind(n) n(#n)
int main(int argc, const char *argv[]) {
  Arguments args {{ "x", 0.5 }};
  
  Parser::Tokenizer t("-ln(cos(x))");
  Parser::Parser p(t.tokens);
  
  t.dump();
  p.dump();
  
  Node ast;
  ast = p.buildAST();
  
  std::cout << "f (x) = " << ast.latex() << std::endl;
  ast.simplify();
  std::cout << "f (x) = " << ast.latex() << std::endl;
  
  std::cout << ast.term->calculate(args) << std::endl;
  
  /*
   ast.expand();
  std::cout << "f (x) = " << ast.latex() << std::endl;
  ast.simplify();
  std::cout << "f (x) = " << ast.latex() << std::endl;*/
  
  Node derivate = ast.derive(Variable("x"));
  
  std::cout << "f'(x) = " << derivate.latex() << std::endl;
  derivate.simplify();
  std::cout << "f'(x) = " << derivate.latex() << std::endl;
  
  return 0;
}