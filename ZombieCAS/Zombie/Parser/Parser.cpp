//
//  Parser.cpp
//  ZombieCAS
//
//  Created by Alexander Rath on 11.10.14.
//  Copyright (c) 2014 Alexander Rath. All rights reserved.
//

#include "Parser.h"
#include <stack>

using namespace Zombie::Parser;

Parser::Parser(const std::vector<Token *> &tokens) {
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