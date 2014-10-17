//
//  Constant.h
//  ZombieCAS
//
//  Created by Alexander Rath on 11.10.14.
//  Copyright (c) 2014 Alexander Rath. All rights reserved.
//

#ifndef __ZombieCAS__Constant__
#define __ZombieCAS__Constant__

#include <iostream>
#include <sstream>

#include "Term.h"

namespace Zombie {
  namespace Terms {
    class Constant : public Term {
    public:
      double n = 1.f;
      unsigned long d = 1;
      
      explicit operator double() { return n / d; }
      
      Constant(double v) : n(v) {}
      
      virtual TermSharedPtr deriveUntidy(const Variable &v) const { return TermSharedPtr(new Constant(0)); }
      Constant orderOf(const Term *) const;
      
      virtual TermSharedPtr tidy(TermSharedPtr &self) {
        // TODO:2014-10-18:alex:Important. Implement this.
        return self;
      }
      
      const std::string latex() const {
        std::ostringstream os;
        os << n;
        if(d != 1) os << "/" << d;
        return os.str();
      }
      
#define def_op(op) \
bool operator op(double other) const { return this->n op other * this->d; } \
bool operator op(const Constant &other) const { return this->n * other.d op other.n * this->d; }
      def_op(==)
      def_op(!=)
      def_op(<)
      def_op(<=)
      def_op(>)
      def_op(>=)
#undef def_op
      
#define def_op(op) \
      Constant &operator op##=(const Constant &other) { \
        this->n = this->n * other.d op other.n * this->d; \
        this->d *= other.d; \
        return *this; \
      }
      
      def_op(+)
      def_op(-)
#undef def_op
      
      Constant &operator *=(const Constant &other) {
        this->n *= other.n;
        this->d *= other.d;
        return *this;
      }
      
      Constant &operator /=(const Constant &other) {
        this->n *= other.d;
        this->d *= other.n;
        return *this;
      }
      
      bool operator ==(const Term &other) const {
        if(dynamic_cast<const Constant *>(&other)) {
          const Constant *that = (const Constant *)&other;
          return this->n * that->d == that->n * this->d;
        } return false;
      }
    };
  }
}

#endif /* defined(__ZombieCAS__Constant__) */