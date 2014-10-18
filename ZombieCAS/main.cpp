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

#include "Node.h"

using namespace Zombie;
using namespace Zombie::Terms;
using namespace Zombie::Definitions;

#define _bind(n) n(#n)
int main(int argc, const char *argv[]) {
  Parser::Tokenizer t("(2^0.5)^3");
  Parser::Parser p(t.tokens);
  
  //t.dump();
  //p.dump();
  
  Node ast;
  ast = p.buildAST();
  
  std::cout << "f (x) = " << ast.latex() << std::endl;
  ast.simplify();
  std::cout << "f (x) = " << ast.latex() << std::endl;
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