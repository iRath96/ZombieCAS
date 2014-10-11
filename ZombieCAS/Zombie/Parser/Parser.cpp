//
//  Parser.cpp
//  ZombieCAS
//
//  Created by Alexander Rath on 11.10.14.
//  Copyright (c) 2014 Alexander Rath. All rights reserved.
//

#include "Parser.h"
#include "Tokenizer.h"

#include "OpAdd.h"
#include "OpMultiply.h"
#include "OpPow.h"
#include "Invocation.h"
#include "Variable.h"
#include "Constant.h"
#include "Function.h"

#include <stack>

using namespace Zombie::Terms;
using namespace Zombie::Parser;
using namespace Zombie::Definitions;

Parser::Parser(std::string str) {
  Tokenizer tokenizer(str);
  buildRPN(tokenizer.tokens);
}

Parser::Parser(const std::vector<Token *> &tokens) {
  buildRPN(tokens);
}

void Parser::buildRPN(const std::vector<Token *> &tokens) {
  std::stack<const Token *> stack;
  
  for(auto it = tokens.begin(); it != tokens.end(); ++it) {
    const Token *token = *it;
    
    if(token->precedence == 0) {
      rpn.push_back(token);
      continue;
    }
    
    while(!stack.empty() && (token->text == ")" || stack.top()->text != "(") && (
                                                                                 stack.top()->precedence >  token->precedence ||
                                                                                 (stack.top()->precedence == token->precedence && !token->rightToLeft))) {
      if(token->text == ")" && stack.top()->text == "(") {
        stack.pop();
        break;
      }
      
      rpn.push_back(stack.top());
      stack.pop();
    } if(token->text != ")") stack.push(token);
  }
  
  while(!stack.empty()) {
    rpn.push_back(stack.top());
    stack.pop();
  }
  
  // And now an abstract syntax tree
  
}

Term *Parser::buildAST() const {
  struct stack_element_t {
    void *element;
    bool isVector;
  };
  
  std::stack<stack_element_t> stack;
  
  for(auto it = rpn.begin(); it != rpn.end(); ++it) {
    const Token *token = *it;
    stack_element_t result;
    
    if(token->isOperator()) {
      if(token->isInvocation()) {
        /* if(stack.empty()); */ // TODO:2014-10-11:alex:Throw an exception here.
        stack_element_t e = stack.top(); stack.pop();
        if(!e.isVector) e = { (void *)new TermVector { (Term *)e.element }, true };
        result = { (void *)new Invocation(new Function(token->text), *(TermVector *)e.element), false };
      } else {
        /* if(stack.size() < 2); */ // TODO:2014-10-11:alex:Throw an exception here.
        
        stack_element_t b = stack.top(); stack.pop();
        stack_element_t a = stack.top(); stack.pop();
        
        switch(token->text[0]) {
          case ',':
            if(a.isVector) result = a;
            else result = { (void *)new TermVector { (Term *)a.element }, true };
            /* if(b.isVector); */ // TODO:2014-10-11:alex:Throw an exception here.
            ((TermVector *)result.element)->push_back((Term *)b.element);
            break;
#define operation(klass) result = { (void *)new klass(TermVector { (Term *)a.element, (Term *)b.element }), false }
          case '*': operation(OpMultiply); break;
          case '^': operation(OpPow); break;
          case '+': operation(OpAdd); break;
#undef operation
#define operation(klass, klass2) \
result = { (void *)new klass(TermVector { \
  (Term *)a.element, \
  (Term *)new klass2(TermVector { (Term *)b.element, new Constant(-1) }) \
}), false }
          case '/': operation(OpMultiply, OpPow); break;
          case '-': operation(OpAdd, OpMultiply); break;
#undef operation
        }
      }
    } else
      result = {
        (token->isLiteral() ?
         (void *)new Constant(std::stod(token->text)) :
         (void *)new Variable(token->text)),
        false };
    stack.push(result);
  }
  
  /* if(stack.size() != 1); */ // TODO:2014-10-11:alex:Throw an exception here.
  /* if(stack.top().isVector); */ // TODO:2014-10-11:alex:Throw an exception here.
  return (Term *)stack.top().element;
}

void Parser::dump() const {
  std::cout << __PRETTY_FUNCTION__ << " with " << rpn.size() << " elements" << std::endl;
  
  int index = 0;
  std::stack<int> stack;
  
  for(auto it = rpn.begin(); it != rpn.end(); ++it) {
    if((*it)->isOperator()) {
      if((*it)->isInvocation()) {
        int i = stack.top(); stack.pop();
        std::cout << "  $" << (++index) << " = " << (*it)->text << " $" << i << std::endl;
      } else {
        int b = stack.top(); stack.pop();
        int a = stack.top(); stack.pop();
        std::cout << "  $" << (++index) << " = $" << a << " " << (*it)->text << " $" << b << std::endl;
      }
    } else
      std::cout << "  $" << (++index) << " = %" << (*it)->text << std::endl;
    stack.push(index);
  } std::cout << "  return $" << stack.top() << std::endl << std::endl;
}