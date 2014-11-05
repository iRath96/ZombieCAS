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

Node ast;
Arguments args {{ "x", 0.0 }};

void taylor(std::string variable, Number center, int degree) {
  OpAdd *sum = new OpAdd();
  
  TermSharedPtr c = TermSharedPtr(new Constant(center));
  TermSharedPtr v = TermSharedPtr(new Variable(variable));
  TermSharedPtr sub = Term::subtract(v, c);
  
  int i = 0, denominator = 1;
  
  while(i <= degree) {
    ast.simplify();
    ast.simplify();
    
    args.find(variable)->second = center;
    Number y = ast.term->calculate(args);
    
    TermSharedPtr exp = TermSharedPtr(new Constant(i));
    TermSharedPtr a = Term::pow(sub, exp);
    TermSharedPtr b = TermSharedPtr(new Constant(y, denominator));
    
    *sum += Term::multiply(a, b);
    
    denominator *= ++i;
    
    ast = ast.derive(variable);
  }
  
  ast.term = TermSharedPtr(sum);
}

#define TIME(name, iterations, block) { \
unsigned long a, b; \
struct timeval tv; \
gettimeofday(&tv, NULL); a = tv.tv_sec * 1000000 + tv.tv_usec; \
for(size_t i = 0; i < iterations; ++i) block \
gettimeofday(&tv, NULL); b = tv.tv_sec * 1000000 + tv.tv_usec - a; \
if(iterations) printf("%s finished in %ldµs", name, b); \
}

#define _bind(n) n(#n)
int main(int argc, const char *argv[]) {
  Arguments args {{ "x", 0.5 }};
  
  Parser::Tokenizer t("(1-sin(x)^2)^(1/2*cos(x))");
  Parser::Parser p(t.tokens);
  
  t.dump();
  p.dump();
  
  ast = p.buildAST();
  
  std::cout << "f (x) = " << ast.latex() << std::endl;
  ast.simplify();
  std::cout << "f (x) = " << ast.latex() << std::endl;
  
  std::cout << ast.term->calculate(args) << std::endl;
  
  /*TIME("taylor", 100, {
    taylor("x", 0, 8);
  });*/
  
  ast = ast.derive(Variable("x"));
  ast.simplify();
  
  std::cout << ast.latex() << std::endl;
  return 0;
  
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