//
//  jsapi.cpp
//  ZombieCAS
//
//  Created by Alexander Rath on 03.11.14.
//  Copyright (c) 2014 Alexander Rath. All rights reserved.
//

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

#include <map>
#include <vector>

#ifdef EMSCRIPTEN
#include <emscripten/bind.h>
using namespace emscripten;
#endif

typedef int context_id_t;

context_id_t counter = 0, current;
std::map<context_id_t, std::vector<Node>> contexts;

context_id_t create() { return current = ++counter; }
void destroy(context_id_t context) { contexts.erase(context); }
void select(context_id_t context) { current = context; }

Node taylorNode;
Node *ast;

void parse(std::string formula) {
  Parser::Tokenizer t(formula);
  Parser::Parser p(t.tokens);
  
  contexts[current].clear();
  contexts[current].push_back(Node(p.buildAST()));
  
  ast = &contexts[current][0];
}

void order(int order) {
  std::vector<Node> &ctx = contexts[current];
  if(ctx.size() <= order) {
    ast = &ctx[ctx.size() - 1];
    while(ctx.size() <= order) {
      ctx.push_back(ast->derive(Variable("x")));
      ast = &ctx[ctx.size() - 1];
      ast->simplify();
      ast->simplify();
    }
  } else
    ast = &ctx[order];
}

// TODO:2014-11-05:alex:Implementy 'destroy'

#pragma mark Useless code

void simplify() { ast->simplify(); }
void expand() { ast->expand(); }

std::string latex() { return ast->latex(); }
std::string toString() { return ast->toString(); }

Arguments args {{ "x", 0.0 }};
double calculateWithX(double x) {
  args.find("x")->second = x;
  return ast->term->calculate(args);
}

Number getParam(std::string name) { return args[name]; }
void setParam(std::string name, Number value) { args[name] = value; }

void taylor(Number center, int degree) {
  OpAdd *sum = new OpAdd();
  
  TermSharedPtr c = TermSharedPtr(new Constant(center));
  TermSharedPtr v = TermSharedPtr(new Variable("x"));
  TermSharedPtr sub = Term::subtract(v, c);
  
  int i = 0, denominator = 1;
  
  while(i <= degree) {
    order(i);
    
    args.find("x")->second = center;
    Number y = ast->term->calculate(args);
    
    TermSharedPtr exp = TermSharedPtr(new Constant(i));
    TermSharedPtr a = Term::pow(sub, exp);
    TermSharedPtr b = TermSharedPtr(new Constant(y, denominator));
    
    *sum += Term::multiply(a, b);
    
    denominator *= ++i;
  }
  
  taylorNode.term = TermSharedPtr(sum);
  ast = &taylorNode;
}

void numericTaylor(std::string variable, Number center, Number epsilon, int degree) {
  Number space[degree+1]; // A space-degree? Sounds complicated to get!
  
  // 4th order: f, f', f'', f''', f''''
  // x^2, e=1.0, d=4
  // x     = 0.0  0.2  0.4  0.6  0.8
  // y     = 0.00 0.04 0.16 0.36 0.64
  // y'    = 0.04 0.12 0.20 0.28
  // y''   = 0.08 0.08 0.08
  // y'''  = 0.00 0.00
  // y'''' = 0.00
  
  for(int i = 0; i <= degree; ++i) {
    Number x = center + epsilon * i / degree;
    
    args.find(variable)->second = x;
    space[i] = ast->term->calculate(args);
  }
  
  OpAdd *sum = new OpAdd();
  
  TermSharedPtr c = TermSharedPtr(new Constant(center));
  TermSharedPtr v = TermSharedPtr(new Variable(variable));
  TermSharedPtr sub = Term::subtract(v, c);
  
  int i = 0, denominator = 1;
  
  while(i <= degree) {
    Number y = space[0];
    
    TermSharedPtr exp = TermSharedPtr(new Constant(i));
    TermSharedPtr a = Term::pow(sub, exp);
    TermSharedPtr b = TermSharedPtr(new Constant(y, denominator));
    
    if(::abs((double)*(Constant *)(b.get())) > 1e-10) *sum += Term::multiply(a, b);
    
    for(int j = 0, k = degree - i; j < k;) {
      space[j] = (space[j+1] - y) * degree / epsilon;
      y = space[++j];
    }
    
    denominator *= ++i;
  }
  
  taylorNode.term = TermSharedPtr(sum);
  ast = &taylorNode;
}

#ifdef EMSCRIPTEN
EMSCRIPTEN_BINDINGS(my_module) {
  function("create", &create);
  function("destroy", &destroy);
  function("select", &select);
  function("order", &order);
  
  function("parse", &parse);
  function("toString", &toString);
  function("latex", &latex);
  function("simplify", &simplify);
  function("expand", &expand);
  function("calculateWithX", &calculateWithX);
  
  function("getParam", &getParam);
  function("setParam", &setParam);
  
  function("taylor", &taylor);
  function("numericTaylor", &numericTaylor);
}
#else
int main(int argc, const char *argv[]) {
  create();
  parse("x^(-a-b)*a");
  std::cout << ast->term->latex({ kLP_ROOT, true }) << std::endl;
  order(1);
  std::cout << latex() << std::endl;
  
  return 0;
}
#endif