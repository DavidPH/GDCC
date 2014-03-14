//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree conversion expression bases.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AST__Exp__Convert_H__
#define GDCC__AST__Exp__Convert_H__

#include "../../AST/Exp/Unary.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Exp_Convert
      //
      class Exp_Convert : public Exp_Unary
      {
         GDCC_Core_CounterPreambleAbstract(
            GDCC::AST::Exp_Convert, GDCC::AST::Exp_Unary);

      protected:
         Exp_Convert(Type const *t, Exp const *e, Core::Origin pos_) :
            Super{t, e, pos_} {}

         virtual IRExpCRef v_getIRExp() const;
      };
   }
}

#endif//GDCC__AST__Exp__Convert_H__

