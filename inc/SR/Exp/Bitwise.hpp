//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2016 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Source Representation bitwise and shift expression bases.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__SR__Exp__Bitwise_H__
#define GDCC__SR__Exp__Bitwise_H__

#include "../../SR/Exp/Arith.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace SR
   {
      //
      // Exp_BitAnd
      //
      class Exp_BitAnd : public Exp_Binary
      {
         GDCC_SR_Exp_ArithPreamble(Exp_BitAnd);
      };

      //
      // Exp_BitOrI
      //
      class Exp_BitOrI : public Exp_Binary
      {
         GDCC_SR_Exp_ArithPreamble(Exp_BitOrI);
      };

      //
      // Exp_BitOrX
      //
      class Exp_BitOrX : public Exp_Binary
      {
         GDCC_SR_Exp_ArithPreamble(Exp_BitOrX);
      };

      //
      // Exp_ShL
      //
      class Exp_ShL : public Exp_Binary
      {
         GDCC_SR_Exp_ArithPreamble(Exp_ShL);
      };

      //
      // Exp_ShR
      //
      class Exp_ShR : public Exp_Binary
      {
         GDCC_SR_Exp_ArithPreamble(Exp_ShR);
      };
   }
}

#endif//GDCC__SR__Exp__Bitwise_H__

