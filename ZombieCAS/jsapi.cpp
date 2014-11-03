//
//  jsapi.cpp
//  ZombieCAS
//
//  Created by Alexander Rath on 03.11.14.
//  Copyright (c) 2014 Alexander Rath. All rights reserved.
//

#include <emscripten/bind.h>

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

using namespace emscripten;

Node ast;
void parse(std::string input) {
  Parser::Tokenizer t(input);
  Parser::Parser p(t.tokens);
  
  ast = p.buildAST();
}

void derive(std::string variable) {
  ast = ast.derive(Variable(variable));
}

void simplify() { ast.simplify(); }
void expand() { ast.expand(); }

std::string latex() {
  return ast.latex();
}

Arguments args {{ "x", 0.0 }};
double calculateWithX(double x) {
  args.find("x")->second = x;
  return ast.term->calculate(args);
}

EMSCRIPTEN_BINDINGS(my_module) {
  function("parse", &parse);
  function("latex", &latex);
  function("simplify", &simplify);
  function("expand", &expand);
  function("derive", &derive);
  function("calculateWithX", &calculateWithX);
}

//int main(int argc, const char *argv[]) { return 0; }