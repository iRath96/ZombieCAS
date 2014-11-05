//
//  Node.h
//  ZombieCAS
//
//  Created by Alexander Rath on 18.10.14.
//  Copyright (c) 2014 Alexander Rath. All rights reserved.
//

#ifndef __ZombieCAS__Node__
#define __ZombieCAS__Node__

#include <iostream>

#include "Term.h"
#include "Function.h"

namespace Zombie {
  namespace Terms {
    class Node {
    public:
      TermSharedPtr term;
      
      Node() {}
      Node(TermSharedPtr term) : term(term) {}
      
      void operator =(TermSharedPtr &t) { term = t; }
      
      void expand(bool tidy = true);
      void simplify(bool tidy = true);
      void tidy();
      
      Node derive(const Variable &var) { return Node(term->derive(var)); }
      
      std::string toString() { return term->toString(); }
      std::string latex() { return term->latex(); }
    };
  }
}

/* 
 * 50 days to go.
 *
 
 
     *             ,
                 _/^\_
                <     >
*                 /.-.\         *
        *        `/&\`                   *
                ,@.*;@,
               /_o.I %_\    *
  *           (`'--:o(_@;
             /`;--.,__ `')             *
            ;@`o % O,*`'`&\ 
      *    (`'--)_@ ;o %'()\      *
           /`;--._`''--._O'@;
          /&*,()~o`;-.,_ `""`)
*          /`,@ ;+& () o*`;-';\
         (`""--.,_0 +% @' &()\
         /-.,_    ``''--....-'`)  *
    *    /@%;o`:;'--,.__   __.'\
        ;*,&(); @ % &^;~`"`o;@();         *
        /(); o^~; & ().o@*&`;&%O\
  jgs   `"="==""==,,,.,="=="==="`
     __.----.(\-''#####---...___...-----._
   '`         \)_`"""""`
           .--' ')
         o(  )_-\
           `"""` `
 */

#endif /* defined(__ZombieCAS__Node__) */