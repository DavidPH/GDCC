//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// C arithmetic expressions.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__AST__ExpCode_H__
#define GDCC__AST__ExpCode_H__

#include "../AST/Type.hpp"

#include "../IR/Code.hpp"


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
         auto sign = t->getSizeBitsS();
         if(t->isCTypeAccum()) switch(t->getSizeWords())
         {
         case  1: return sign ? Codes::CodeX_W : Codes::CodeK_W;
         default: return IR::Code::None;
         }
         else switch(t->getSizeWords())
         {
         case  1: return sign ? Codes::CodeR_W : Codes::CodeA_W;
         default: return IR::Code::None;
         }
      }

      //
      // ExpCode_ArithFloat
      //
      template<typename Codes>
      IR::Code ExpCode_ArithFloat(Type const *t)
      {
         switch(t->getSizeWords())
         {
         case  1: return Codes::CodeF_W;
         default: return IR::Code::None;
         }
      }

      //
      // ExpCode_ArithInteg
      //
      template<typename Codes>
      IR::Code ExpCode_ArithInteg(Type const *t)
      {
         auto sign = t->getSizeBitsS();
         switch(t->getSizeWords())
         {
         case  1: return sign ? Codes::CodeI_W : Codes::CodeU_W;
         default: return IR::Code::None;
         }
      }

      //
      // ExpCode_ArithPoint
      //
      template<typename Codes>
      IR::Code ExpCode_ArithPoint(Type const *t)
      {
         switch(t->getSizeWords())
         {
         case  1: return Codes::CodeU_W;
         default: return IR::Code::None;
         }
      }
   }
}

#endif//GDCC__AST__ExpCode_H__

