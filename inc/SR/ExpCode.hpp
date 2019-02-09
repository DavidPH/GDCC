//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2019 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Expression code functions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__SR__ExpCode_H__
#define GDCC__SR__ExpCode_H__

#include "../SR/Type.hpp"

#include "../IR/Code.hpp"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

namespace GDCC::SR
{
   //
   // ExpCode_ArithFixed
   //
   inline IR::Code ExpCode_ArithFixed(IR::CodeBase base, Type const *t)
   {
      if(t->isCTypeAccum())
      {
         if(t->getSizeBitsS())
            return {base, 'X'};
         else
            return {base, 'K'};
      }
      else
      {
         if(t->getSizeBitsS())
            return {base, 'R'};
         else
            return {base, 'A'};
      }
   }

   //
   // ExpCode_ArithFloat
   //
   inline IR::Code ExpCode_ArithFloat(IR::CodeBase base, Type const *t)
   {
      if(t->getSizeBitsS())
         return {base, 'F'};
      else
         return {base, 'E'};
   }

   //
   // ExpCode_ArithInteg
   //
   inline IR::Code ExpCode_ArithInteg(IR::CodeBase base, Type const *t)
   {
      if(t->getSizeBitsS())
         return {base, 'I'};
      else
         return {base, 'U'};
   }

   //
   // ExpCode_ArithPoint
   //
   inline IR::Code ExpCode_ArithPoint(IR::CodeBase base, Type const *)
   {
      return {base, 'U'};
   }

   //
   // ExpCode_Arith
   //
   inline IR::Code ExpCode_Arith(IR::CodeBase base, Type const *t)
   {
      if(t->isCTypeInteg())
         return ExpCode_ArithInteg(base, t);

      if(t->isCTypeFixed())
         return ExpCode_ArithFixed(base, t);

      if(t->isCTypeRealFlt())
         return ExpCode_ArithFloat(base, t);

      return IR::CodeBase::None;
   }
}

#endif//GDCC__SR__ExpCode_H__

