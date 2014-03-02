//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree unary expression base.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AST__Exp__Unary_H__
#define GDCC__AST__Exp__Unary_H__

#include "../../AST/Exp.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Exp_Unary
      //
      class Exp_Unary : public Exp
      {
         GDCC_Core_CounterPreambleAbstract(
            GDCC::AST::Exp_Unary, GDCC::AST::Exp);

      public:
         Exp::CRef const exp;

      protected:
         Exp_Unary(Exp const *e, Core::Origin pos);

         virtual bool v_isEffect() const;

         virtual bool v_isIRExp() const;
      };
   }
}

#endif//GDCC__AST__Exp__Binary_H__

