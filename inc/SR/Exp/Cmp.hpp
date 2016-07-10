//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation comparator expression bases.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__SR__Exp__Cmp_H__
#define GDCC__SR__Exp__Cmp_H__

#include "../../SR/Exp/Arith.hpp"


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// GDCC_SR_Exp_CmpPreamble
//
#define GDCC_SR_Exp_CmpPreamble(class) \
   GDCC_SR_Exp_ArithPreamble(class); \
   \
protected: \
   virtual bool v_isBoolean() const


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace SR
   {
      //
      // Exp_CmpEQ
      //
      class Exp_CmpEQ : public Exp_Binary
      {
         GDCC_SR_Exp_CmpPreamble(Exp_CmpEQ);
      };

      //
      // Exp_CmpGE
      //
      class Exp_CmpGE : public Exp_Binary
      {
         GDCC_SR_Exp_CmpPreamble(Exp_CmpGE);
      };

      //
      // Exp_CmpGT
      //
      class Exp_CmpGT : public Exp_Binary
      {
         GDCC_SR_Exp_CmpPreamble(Exp_CmpGT);
      };

      //
      // Exp_CmpLE
      //
      class Exp_CmpLE : public Exp_Binary
      {
         GDCC_SR_Exp_CmpPreamble(Exp_CmpLE);
      };

      //
      // Exp_CmpLT
      //
      class Exp_CmpLT : public Exp_Binary
      {
         GDCC_SR_Exp_CmpPreamble(Exp_CmpLT);
      };

      //
      // Exp_CmpNE
      //
      class Exp_CmpNE : public Exp_Binary
      {
         GDCC_SR_Exp_CmpPreamble(Exp_CmpNE);
      };
   }
}

#endif//GDCC__SR__Exp__Cmp_H__

