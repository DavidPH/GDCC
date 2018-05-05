//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
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
   template<typename Codes>
   IR::Code ExpCode_ArithFixed(Type const *t)
   {
      if(t->isCTypeAccum())
      {
         if(t->getSizeBitsS())
            return Codes::CodeX;
         else
            return Codes::CodeK;
      }
      else
      {
         if(t->getSizeBitsS())
            return Codes::CodeR;
         else
            return Codes::CodeA;
      }
   }

   //
   // ExpCode_ArithFloat
   //
   template<typename Codes>
   IR::Code ExpCode_ArithFloat(Type const *t)
   {
      if(t->getSizeBitsS())
         return Codes::CodeF;
      else
         return IR::Code::None;
   }

   //
   // ExpCode_ArithInteg
   //
   template<typename Codes>
   IR::Code ExpCode_ArithInteg(Type const *t)
   {
      if(t->getSizeBitsS())
         return Codes::CodeI;
      else
         return Codes::CodeU;
   }

   //
   // ExpCode_ArithPoint
   //
   template<typename Codes>
   IR::Code ExpCode_ArithPoint(Type const *)
   {
      return Codes::CodeU;
   }

   //
   // ExpCode_Arith
   //
   template<typename Codes>
   IR::Code ExpCode_Arith(Type const *t)
   {
      if(t->isCTypeInteg())
         return ExpCode_ArithInteg<Codes>(t);

      if(t->isCTypeFixed())
         return ExpCode_ArithFixed<Codes>(t);

      if(t->isCTypeRealFlt())
         return ExpCode_ArithFloat<Codes>(t);

      return IR::Code::None;
   }
}

#endif//GDCC__SR__ExpCode_H__

