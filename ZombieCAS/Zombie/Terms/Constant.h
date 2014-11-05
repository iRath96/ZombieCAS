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
      
      Constant(double n, unsigned long d = 1) : n(n), d(d) {}
      
      virtual TermSharedPtr deriveUntidy(const Variable &v) const { return TermSharedPtr(new Constant(0)); }
      Constant orderOf(const Term *) const;
      
      TermSharedPtr tidy(TermSharedPtr &);
      
      Number calculate(const Arguments &) const { return n / d; }
      
      const short sign() const {
        return n < 0 ? -1 : +1;
      }
      
      const std::string toString() const {
        std::ostringstream os;
        os << n;
        if(d != 1) os << "/" << d;
        return os.str();
      }
      
      const std::string latex(const latex_ctx_t &ctx) const {
        std::ostringstream os;
        if(d == 1) os << n * (ctx.negate ? -1 : +1);
        else os << "\\frac{" << n * (ctx.negate ? -1 : +1) << "}{" << d << "}";
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
      
      // TODO:2014-11-04:alex:This does not work! [ this->d ] is long and [ other.n ] is double!!
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