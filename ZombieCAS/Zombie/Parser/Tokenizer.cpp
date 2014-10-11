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
  
  for(int i = 0; i < input.length(); ++i) {
    char chr = input[i];
    
    if(chr == ' ') {
      if(memory) goto acceptToken;
      else continue;
    }
    
    if(chr == '(' && memory) {
      memory->invoke();
      i -= 1;
      goto acceptToken;
    }
    
    if(chr == '^' ||
       chr == '*' || chr == '/' ||
       chr == '+' || chr == '-' ||
       chr == ')' || chr == ',') if(memory) {
      i -= 1;
      goto acceptToken;
    }
    
    if(memory) memory->append(chr);
    else memory = new Token(chr);
    
    if(memory->precedence) goto acceptToken;
    
    if(i < input.length() - 1) continue;
    
  acceptToken:
    if(needsMul && memory->hasValue()) tokens.push_back(new Token('*'));
    
    tokens.push_back(memory);
    needsMul = memory->precedence == 0 || memory->text == ")";
    memory = NULL;
  }
}

void Tokenizer::dump() const {
  std::cout << __PRETTY_FUNCTION__ << " with " << tokens.size() << " elements" << std::endl << "  ";
  for(auto it = tokens.begin(); it != tokens.end(); ++it)
    std::cout << (*it)->text << "[" << (*it)->precedence << "]" << " ";
  std::cout << std::endl << std::endl;
}