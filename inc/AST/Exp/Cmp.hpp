//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Abstract Syntax Tree comparator expression bases.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AST__Exp__Cmp_H__
#define GDCC__AST__Exp__Cmp_H__

#include "../../AST/Exp/Arith.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace AST
   {
      //
      // Exp_CmpEQ
      //
      class Exp_CmpEQ : public Exp_Binary
      {
         GDCC_AST_Exp_ArithPreamble(Exp_CmpEQ);
      };

      //
      // Exp_CmpGE
      //
      class Exp_CmpGE : public Exp_Binary
      {
         GDCC_AST_Exp_ArithPreamble(Exp_CmpGE);
      };

      //
      // Exp_CmpGT
      //
      class Exp_CmpGT : public Exp_Binary
      {
         GDCC_AST_Exp_ArithPreamble(Exp_CmpGT);
      };

      //
      // Exp_CmpLE
      //
      class Exp_CmpLE : public Exp_Binary
      {
         GDCC_AST_Exp_ArithPreamble(Exp_CmpLE);
      };

      //
      // Exp_CmpLT
      //
      class Exp_CmpLT : public Exp_Binary
      {
         GDCC_AST_Exp_ArithPreamble(Exp_CmpLT);
      };

      //
      // Exp_CmpNE
      //
      class Exp_CmpNE : public Exp_Binary
      {
         GDCC_AST_Exp_ArithPreamble(Exp_CmpNE);
      };
   }
}

#endif//GDCC__AST__Exp__Cmp_H__

