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
#include "../IR/OpCode.hpp"


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
      IR::OpCode ExpCode_ArithFixed(Type const *t)
      {
         if(t->isCTypeAccum())
         {
            if(t->getSizeBitsS())
               return {Codes::CodeX_W, t->getSizeWords()};
            else
               return {Codes::CodeK_W, t->getSizeWords()};
         }
         else
         {
            if(t->getSizeBitsS())
               return {Codes::CodeR_W, t->getSizeWords()};
            else
               return {Codes::CodeA_W, t->getSizeWords()};
         }
      }

      //
      // ExpCode_ArithFloat
      //
      template<typename Codes>
      IR::OpCode ExpCode_ArithFloat(Type const *t)
      {
         return {Codes::CodeF_W, t->getSizeWords()};
      }

      //
      // ExpCode_ArithInteg
      //
      template<typename Codes>
      IR::OpCode ExpCode_ArithInteg(Type const *t)
      {
         if(t->getSizeBitsS())
            return {Codes::CodeI_W, t->getSizeWords()};
         else
            return {Codes::CodeU_W, t->getSizeWords()};
      }

      //
      // ExpCode_ArithPoint
      //
      template<typename Codes>
      IR::OpCode ExpCode_ArithPoint(Type const *t)
      {
         return {Codes::CodeU_W, t->getSizeWords()};
      }

      //
      // ExpCode_Arith
      //
      template<typename Codes>
      IR::OpCode ExpCode_Arith(Type const *t)
      {
         if(t->isCTypeInteg())
            return ExpCode_ArithInteg<Codes>(t);

         if(t->isCTypeFixed())
            return ExpCode_ArithFixed<Codes>(t);

         if(t->isCTypeRealFlt())
            return ExpCode_ArithFloat<Codes>(t);

         return {IR::Code::None, 0};
      }
   }
}

#endif//GDCC__AST__ExpCode_H__

