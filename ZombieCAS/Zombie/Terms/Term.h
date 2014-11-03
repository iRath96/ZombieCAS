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
#include <map>

namespace Zombie {
  namespace Terms {
    class OpAdd;
    class OpMultiply;
    class OpPow;
    
    class Constant;
    class Variable;
    
    class Term;
    
    typedef enum {
      kLP_ROOT = 0,
      kLP_ADD,
      kLP_MULTIPLY,
      kLP_POWER_BASE
    } latex_precedence_t;
    
    typedef struct __class {
      latex_precedence_t parentalPrecedence = kLP_ROOT;
      bool negate = false;
      
      __class(latex_precedence_t p = kLP_ROOT, bool n = false) : parentalPrecedence(p), negate(n) {}
    } latex_ctx_t;
    
    typedef double Number;
    typedef std::map<std::string, Number> Arguments;
    
    typedef Term *TermPtr;
    typedef std::shared_ptr<Term> TermSharedPtr;
    typedef std::vector<TermPtr> TermVector;
    typedef std::vector<TermSharedPtr> TermVectorShared;
    
    class Term {
    public:
      operator TermPtr() { return this; };
      
#define def_op(op, name) \
static TermSharedPtr name(TermSharedPtr &a, TermSharedPtr &b); \
//klass & operator op(TermSharedPtr other) { return name(other); } \
//template<class T> Term &operator op##=(T other) { std::cout << "op" #op "=" << std::endl; return *this = name(other); }
      def_op(*, multiply)
      def_op(/, divide)
      def_op(+, add)
      def_op(-, subtract)
      def_op(^, pow)
#undef def_op
      
      Term &operator=(const Term *rhs) {
        std::cout << "Magic" << std::endl;
        if(this != rhs) {
          // TODO:2014-10-09:alex:Do some magic here.
        } return *this;
      };
      
      // TODO:2014-10-09:alex:Copy constructor.
      
      //virtual Constant orderOf(const Term *) const = 0;
      
      virtual TermSharedPtr deriveUntidy(const Variable &) const = 0;
      TermSharedPtr derive(const Variable &var) {
        TermSharedPtr derivate = deriveUntidy(var);
        return derivate->tidy(derivate);
      }
      
      virtual ~Term() {}
      
      virtual TermSharedPtr tidy(TermSharedPtr &self) = 0;
      virtual TermSharedPtr simplify(TermSharedPtr &self) { return self; }
      virtual TermSharedPtr expand(TermSharedPtr &self) { return self; }
      
      virtual Number calculate(const Arguments &) const = 0;
      
      virtual const short sign() const = 0;
      virtual const std::string toString() const = 0;
      virtual const std::string latex(const latex_ctx_t &) const = 0;
      const std::string latex() { return latex((latex_ctx_t){}); }
      
      virtual bool operator !=(const Term &other) { return !(*this == other); }
      virtual bool operator ==(const Term &other) const = 0;
    };
    
    struct TSPKey {
      TermSharedPtr ptr;
      bool operator ==(const TSPKey &other) const { return *ptr == *(other.ptr); }
    };
  }
}

namespace std {
  template <>
  struct hash<Zombie::Terms::TSPKey> {
    std::size_t operator()(const Zombie::Terms::TSPKey &key) const {
      return typeid(*key.ptr.get()).hash_code();
    }
  };
}

#endif /* defined(__ZombieCAS__Term__) */