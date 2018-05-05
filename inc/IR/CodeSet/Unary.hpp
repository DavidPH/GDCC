//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation unary code sets.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__CodeSet__Unary_H__
#define GDCC__IR__CodeSet__Unary_H__

#include "../../IR/Code.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::IR
{
   //
   // CodeSet_Inv
   //
   struct CodeSet_Inv
   {
      static constexpr Code
         CodeI = Code::BNot,
         CodeU = Code::BNot;
   };

   //
   // CodeSet_Neg
   //
   struct CodeSet_Neg
   {
      static constexpr Code
         CodeA = Code::NegI,
         CodeF = Code::NegF,
         CodeI = Code::NegI,
         CodeK = Code::NegI,
         CodeR = Code::NegI,
         CodeU = Code::NegI,
         CodeX = Code::NegI;
   };
}

#endif//GDCC__IR__CodeSet__Unary_H__

