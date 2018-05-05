//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation bitwise and shift code sets.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__CodeSet__Bitwise_H__
#define GDCC__IR__CodeSet__Bitwise_H__

#include "../../IR/Code.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::IR
{
   //
   // CodeSet_And
   //
   struct CodeSet_And
   {
      static constexpr Code
         CodeI = Code::BAnd,
         CodeU = Code::BAnd;
   };

   //
   // CodeSet_OrI
   //
   struct CodeSet_OrI
   {
      static constexpr Code
         CodeI = Code::BOrI,
         CodeU = Code::BOrI;
   };

   //
   // CodeSet_OrX
   //
   struct CodeSet_OrX
   {
      static constexpr Code
         CodeI = Code::BOrX,
         CodeU = Code::BOrX;
   };

   //
   // CodeSet_ShL
   //
   struct CodeSet_ShL
   {
      static constexpr Code
         CodeA = Code::ShLU,
         CodeF = Code::ShLF,
         CodeI = Code::ShLU,
         CodeK = Code::ShLU,
         CodeR = Code::ShLU,
         CodeU = Code::ShLU,
         CodeX = Code::ShLU;
   };

   //
   // CodeSet_ShR
   //
   struct CodeSet_ShR
   {
      static constexpr Code
         CodeA = Code::ShRU,
         CodeF = Code::ShRF,
         CodeI = Code::ShRI,
         CodeK = Code::ShRU,
         CodeR = Code::ShRI,
         CodeU = Code::ShRU,
         CodeX = Code::ShRI;
   };
}

#endif//GDCC__IR__CodeSet__Bitwise_H__

