//
//  Term.h
//  ZombieCAS
//
//  Created by Alexander Rath on 11.10.14.
//  Copyright (c) 2014 Alexander Rath. All rights reserved.
//

#ifndef __ZombieCAS__Term__
#define __ZombieCAS__Term__

#include <iostream>
#include <vector>

namespace Zombie {
  namespace Terms {
    class OpAdd;
    class OpMultiply;
    class OpPow;
    
    class Constant;
    
    class Term {
    public:
      operator Term *() { return this; };
      
#define def_op(op, name, klass) \
virtual klass & name(Term *t); \
virtual klass & name(double v); \
klass & operator op(Term *other) { return name(other); } \
//template<class T> Term &operator op##=(T other) { std::cout << "op" #op "=" << std::endl; return *this = name(other); }
      def_op(*, multiply, OpMultiply)
      def_op(/, divide,   OpMultiply)
      def_op(+, add,      OpAdd)
      def_op(-, subtract, OpAdd)
      def_op(^, pow,      OpPow)
#undef def_op
      
      Term &operator=(const Term *rhs) {
        std::cout << "Magic" << std::endl;
        if(this != rhs) {
          // TODO:2014-10-09:alex:Do some magic here.
        } return *this;
      };
      
      // TODO:2014-10-09:alex:Copy constructor.
      // TODO:2014-10-09:alex:Expand and simplify.
      
      //virtual Constant orderOf(const Term *) const = 0;
      
      virtual void tidy() = 0;
      virtual const std::string latex() const = 0;
    };
    
    typedef std::vector<Term *> TermVector;
    typedef std::vector<std::unique_ptr<Term>> TermVectorManaged;
  }
}

#endif /* defined(__ZombieCAS__Term__) */