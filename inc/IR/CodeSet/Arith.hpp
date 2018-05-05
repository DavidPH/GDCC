//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation arithmetic code sets.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__CodeSet__Arith_H__
#define GDCC__IR__CodeSet__Arith_H__

#include "../../IR/Code.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::IR
{
   //
   // CodeSet_Add
   //
   struct CodeSet_Add
   {
      static constexpr Code
         CodeA = Code::AddU,
         CodeF = Code::AddF,
         CodeI = Code::AddI,
         CodeK = Code::AddU,
         CodeR = Code::AddI,
         CodeU = Code::AddU,
         CodeX = Code::AddI;
   };

   //
   // CodeSet_DiX
   //
   struct CodeSet_DiX
   {
      static constexpr Code
         CodeA = Code::None,
         CodeF = Code::None,
         CodeI = Code::DiXI,
         CodeK = Code::None,
         CodeR = Code::None,
         CodeU = Code::DiXU,
         CodeX = Code::None;
   };

   //
   // CodeSet_Div
   //
   struct CodeSet_Div
   {
      static constexpr Code
         CodeA = Code::DivA,
         CodeF = Code::DivF,
         CodeI = Code::DivI,
         CodeK = Code::DivK,
         CodeR = Code::DivR,
         CodeU = Code::DivU,
         CodeX = Code::DivX;
   };

   //
   // CodeSet_Mod
   //
   struct CodeSet_Mod
   {
      static constexpr Code
         CodeA = Code::ModU,
         CodeF = Code::None,
         CodeI = Code::ModI,
         CodeK = Code::ModU,
         CodeR = Code::ModI,
         CodeU = Code::ModU,
         CodeX = Code::ModI;
   };

   //
   // CodeSet_Mul
   //
   struct CodeSet_Mul
   {
      static constexpr Code
         CodeA = Code::MulA,
         CodeF = Code::MulF,
         CodeI = Code::MulI,
         CodeK = Code::MulK,
         CodeR = Code::MulR,
         CodeU = Code::MulU,
         CodeX = Code::MulX;
   };

   //
   // CodeSet_Sub
   //
   struct CodeSet_Sub
   {
      static constexpr Code
         CodeA = Code::SubU,
         CodeF = Code::SubF,
         CodeI = Code::SubI,
         CodeK = Code::SubU,
         CodeR = Code::SubI,
         CodeU = Code::SubU,
         CodeX = Code::SubI;
   };
}

#endif//GDCC__IR__CodeSet__Arith_H__

