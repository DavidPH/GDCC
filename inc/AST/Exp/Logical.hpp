//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree logical expression bases.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AST__Exp__Logical_H__
#define GDCC__AST__Exp__Logical_H__

#include "../../AST/Exp/Binary.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Exp_LogAnd
      //
      class Exp_LogAnd : public Exp_Binary
      {
         GDCC_Core_CounterPreamble(
            GDCC::AST::Exp_LogAnd, GDCC::AST::Exp_Binary);

         GDCC_AST_Exp_BinaryCreator(Exp_LogAnd);

      protected:
         virtual void v_genStmnt(GenStmntCtx const &ctx, Arg const &dst) const;

         virtual IRExpCRef v_getIRExp() const;
      };

      //
      // Exp_LogOrI
      //
      class Exp_LogOrI : public Exp_Binary
      {
         GDCC_Core_CounterPreamble(
            GDCC::AST::Exp_LogOrI, GDCC::AST::Exp_Binary);

         GDCC_AST_Exp_BinaryCreator(Exp_LogOrI);

      protected:
         virtual void v_genStmnt(GenStmntCtx const &ctx, Arg const &dst) const;

         virtual IRExpCRef v_getIRExp() const;
      };
   }
}

#endif//GDCC__AST__Exp__Logical_H__

