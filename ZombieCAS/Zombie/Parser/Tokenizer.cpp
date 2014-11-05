//
//  Tokenizer.cpp
//  ZombieCAS
//
//  Created by Alexander Rath on 11.10.14.
//  Copyright (c) 2014 Alexander Rath. All rights reserved.
//

#include "Tokenizer.h"

using namespace Zombie::Parser;

#define IS_OP(c) ()
Tokenizer::Tokenizer(std::string input) {
  Token *memory = NULL;
  bool needsMul = false;
  bool canBeUnary = true;
  
  for(int i = 0; i < input.length(); ++i) {
    char chr = input[i];
    
    if(memory && ((chr >= '0' && chr <= '9') || chr == '.' || (chr == '-' && canBeUnary)) != memory->isLiteral()) {
      i -= 1;
      goto acceptToken;
    }
    
    if(chr == ' ') {
      if(memory) goto acceptToken;
      else continue;
    }
    
    if(chr == '(') {
      if(memory) {
        memory->invoke();
        i -= 1;
        goto acceptToken;
      } else {
        memory = new Token(chr);
        goto acceptToken;
      }
    }
    
    if(chr == '^' ||
       chr == '*' || chr == '/' ||
       chr == '+' || (chr == '-' && !canBeUnary) ||
       chr == ')' || chr == ',') {
      if(memory) {
        i -= 1;
        goto acceptToken;
      } else if(chr != ')') {
        memory = new Token(chr);
        goto acceptToken;
      }
    }
    
    if(memory) memory->append(chr);
    else {
      memory = new Token(chr);
      if(canBeUnary) memory->precedence = 0;
    }
    
    if(memory->precedence) goto acceptToken;
    
    canBeUnary = false;
    if(i < input.length() - 1) continue;
    
  acceptToken:
    if(memory->text == "^" && !tokens.empty()) {
      for(auto rit = tokens.rbegin(); rit != tokens.rend(); ++rit)
        // Power has higher precedence than a previous implicit multiplication ( 2x^2 => 2*x^2 )
        if((*rit)->precedence) {
          if((*rit)->precedence == 6) {
            // But not if it was implicit due to a unary minus ( 2^-b^2 => 2^(-b^2) )
            if((rit+1) == tokens.rend() || (*(rit+1))->text != "-")
              (*rit)->precedence = 3;
            // If this power follows a unary minus, however, it should bind stronger
            // than the unary minus ( 2^-a^2 => 2^(-(a^2)) )
            else
              // TODO:2014-11-03:alex:The line below is wrong and you know it. (miscounted as invocation)
              memory->precedence = 7;
          }
          break;
        }
    }
    
    if(needsMul && memory->hasValue()) {
      Token *aux = new Token('*');
      if(tokens.back()->isLiteral()) aux->precedence = 6;
      tokens.push_back(aux);
    }
    
    tokens.push_back(memory);
    needsMul = memory->precedence == 0 || memory->text == ")";
    
    canBeUnary = memory->text == "(" || memory->text == "^" || memory->text == "*" || memory->text == "+";
    memory = NULL;
  }
}

void Tokenizer::dump() const {
  std::cout << __PRETTY_FUNCTION__ << " with " << tokens.size() << " elements" << std::endl << "  ";
  for(auto it = tokens.begin(); it != tokens.end(); ++it)
    std::cout << (*it)->text << "[" << (*it)->precedence << "]" << " ";
  std::cout << std::endl << std::endl;
}