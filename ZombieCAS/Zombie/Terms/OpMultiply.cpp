//
//  OpMultiply.cpp
//  ZombieCAS
//
//  Created by Alexander Rath on 11.10.14.
//  Copyright (c) 2014 Alexander Rath. All rights reserved.
//

#include "OpMultiply.h"

#include "Constant.h"
#include "OpAdd.h"
#include "OpPow.h"

#include <unordered_map>

using namespace Zombie::Terms;

TermSharedPtr OpMultiply::deriveUntidy(const Variable &var) const {
  OpAdd *result = new OpAdd();
  for(auto it = operands.begin(); it != operands.end(); ++it) {
    OpMultiply *summand = new OpMultiply();
    for(auto it2 = operands.begin(); it2 != operands.end(); ++it2) {
      TermSharedPtr blah = *it == *it2 ? (*it2)->deriveUntidy(var) : *it2;
      *summand *= blah;
    }
    *result += TermSharedPtr(summand);
  } return TermSharedPtr(result);
}

TermSharedPtr OpMultiply::simplify(TermSharedPtr &self) {
  std::unordered_map<TSPKey, OpAdd *> exponents;
  for(auto it = operands.begin(); it != operands.end(); ++it) {
    TermSharedPtr base = (*it)->simplify(*it);
    TermSharedPtr exp;
    
    if(dynamic_cast<OpPow *>(base.get())) {
      exp = ((OpPow *)base.get())->operands[1];
      base = ((OpPow *)base.get())->operands[0];
    } else
      exp = TermSharedPtr(new Constant(1));
    
    TSPKey key = { base };
    
    if(exponents.find(key) == exponents.end()) exponents[key] = new OpAdd(TermVectorShared { exp });
    else exponents[key]->operands.push_back(exp);
  }
  
  operands.clear();
  
  //std::cout << "= start ===" << std::endl;
  
  for(auto it = exponents.begin(); it != exponents.end(); ++it) {
    TermSharedPtr base = it->first.ptr;
    TermSharedPtr exp = TermSharedPtr(it->second);
    exp = exp->tidy(exp);
    
    //std::cout << base->latex() << " => " << exp->latex() << std::endl;
    
    if(dynamic_cast<Constant *>(exp.get())) {
      if(*(Constant *)exp.get() == 1) {
        operands.push_back(base);
        continue;
      } else if(*(Constant *)exp.get() == 0) continue;
    }
    
    operands.push_back(Term::pow(base, exp));
  }
  
  return self->tidy(self);
}

TermSharedPtr OpMultiply::expand(TermSharedPtr &self) {
  OpAdd *add = new OpAdd();
  for(auto it = operands.begin(); it != operands.end(); ++it) {
    if(dynamic_cast<OpAdd *>(it->get())) {
      OpAdd *factor = (OpAdd *)it->get();
      
      if(add->operands.size() == 0) {
        for(auto it2 = factor->operands.begin(); it2 != factor->operands.end(); ++it2)
          add->operands.push_back(*it2);
        continue;
      }
      
      OpAdd *newAdd = new OpAdd();
      
      // TODO:2014-10-18:alex:This is inefficient. There should be a *= operator.
      for(auto it2 = add->operands.begin(); it2 != add->operands.end(); ++it2)
        for(auto it3 = factor->operands.begin(); it3 != factor->operands.end(); ++it3)
          newAdd->operands.push_back(Term::multiply(*it2, *it3));
      
      delete add;
      add = newAdd;
    } else
      if(add->operands.size() == 0) add->operands.push_back(*it);
      else
        for(size_t i = 0, j = add->operands.size(); i < j; ++i)
          add->operands[i] = Term::multiply(add->operands[i], *it);
  }
  
  return TermSharedPtr(add);
}

const std::string OpMultiply::latex(const latex_ctx_t &ctx) const {
  std::ostringstream osNominator, osDenominator;
  std::ostringstream osConstN, osConstD;
  
  latex_ctx_t localCtx(kLP_MULTIPLY);
  for(auto it = operands.begin(); it != operands.end(); ++it) {
    localCtx.negate = (*it)->sign() == -1;
    if(dynamic_cast<OpPow *>(it->get()))
      if((localCtx.negateExponent = ((OpPow *)it->get())->operands[1]->sign() == -1)) {
        osDenominator << (*it)->latex(localCtx);
        continue;
      }
    
    if(dynamic_cast<Constant *>(it->get())) {
      Constant *c = (Constant *)it->get();
      if(!osConstN.str().empty()) osConstN << "\\cdot";
      osConstN << (c->n < 0 ? -c->n : c->n);
      if(c->d != 1) {
        if(!osConstD.str().empty()) osConstD << "\\cdot";
        osConstD << c->d;
      }
      
      continue;
    }
    
    osNominator << (*it)->latex(localCtx);
  }
  
  bool needsMinus = (sign() == -1) != ctx.negate;
  
  std::ostringstream osFinal;
  
  if(needsMinus) osFinal << "-";
  if(ctx.parentalPrecedence > kLP_MULTIPLY) osFinal << "(";
  
  if(osConstN.str() == "1" && !osNominator.str().empty()) osConstN.str("");
  
  osConstN << osNominator.str();
  osConstD << osDenominator.str();
  
  if(osConstN.str().empty()) osConstN << "1";
  if(osConstD.str().empty())
    osFinal << osConstN.str();
  else
    osFinal << "\\frac{" << osConstN.str() << "}{" << osConstD.str() << "}";
  
  if(ctx.parentalPrecedence > kLP_MULTIPLY) osFinal << ")";
  return osFinal.str();
}