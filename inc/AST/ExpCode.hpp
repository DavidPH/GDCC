//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Expression code functions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AST__ExpCode_H__
#define GDCC__AST__ExpCode_H__

#include "../AST/Type.hpp"

#include "../IR/ExpCode.hpp"


//----------------------------------------------------------------------------|
// Global Functions                                                           |
//

namespace GDCC
{
   namespace AST
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
               return IR::ExpCode_X_W<Codes>(t->getSizeWords());
            else
               return IR::ExpCode_K_W<Codes>(t->getSizeWords());
         }
         else
         {
            if(t->getSizeBitsS())
               return IR::ExpCode_R_W<Codes>(t->getSizeWords());
            else
               return IR::ExpCode_A_W<Codes>(t->getSizeWords());
         }
      }

      //
      // ExpCode_ArithFloat
      //
      template<typename Codes>
      IR::Code ExpCode_ArithFloat(Type const *t)
      {
         return IR::ExpCode_F_W<Codes>(t->getSizeWords());
      }

      //
      // ExpCode_ArithInteg
      //
      template<typename Codes>
      IR::Code ExpCode_ArithInteg(Type const *t)
      {
         if(t->getSizeBitsS())
            return IR::ExpCode_I_W<Codes>(t->getSizeWords());
         else
            return IR::ExpCode_U_W<Codes>(t->getSizeWords());
      }

      //
      // ExpCode_ArithPoint
      //
      template<typename Codes>
      IR::Code ExpCode_ArithPoint(Type const *t)
      {
         return IR::ExpCode_U_W<Codes>(t->getSizeWords());
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
}

#endif//GDCC__AST__ExpCode_H__

