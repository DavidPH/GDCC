//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2018 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Intermediary Representation comparator code sets.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__IR__CodeSet__Cmp_H__
#define GDCC__IR__CodeSet__Cmp_H__

#include "../../IR/Code.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::IR
{
   //
   // CodeSet_CmpEQ
   //
   struct CodeSet_CmpEQ
   {
      static constexpr Code
         CodeA = Code::CmpU_EQ,
         CodeF = Code::CmpF_EQ,
         CodeI = Code::CmpI_EQ,
         CodeK = Code::CmpU_EQ,
         CodeR = Code::CmpI_EQ,
         CodeU = Code::CmpU_EQ,
         CodeX = Code::CmpI_EQ;
   };

   //
   // CodeSet_CmpGE
   //
   struct CodeSet_CmpGE
   {
      static constexpr Code
         CodeA = Code::CmpU_GE,
         CodeF = Code::CmpF_GE,
         CodeI = Code::CmpI_GE,
         CodeK = Code::CmpU_GE,
         CodeR = Code::CmpI_GE,
         CodeU = Code::CmpU_GE,
         CodeX = Code::CmpI_GE;
   };

   //
   // CodeSet_CmpGT
   //
   struct CodeSet_CmpGT
   {
      static constexpr Code
         CodeA = Code::CmpU_GT,
         CodeF = Code::CmpF_GT,
         CodeI = Code::CmpI_GT,
         CodeK = Code::CmpU_GT,
         CodeR = Code::CmpI_GT,
         CodeU = Code::CmpU_GT,
         CodeX = Code::CmpI_GT;
   };

   //
   // CodeSet_CmpLE
   //
   struct CodeSet_CmpLE
   {
      static constexpr Code
         CodeA = Code::CmpU_LE,
         CodeF = Code::CmpF_LE,
         CodeI = Code::CmpI_LE,
         CodeK = Code::CmpU_LE,
         CodeR = Code::CmpI_LE,
         CodeU = Code::CmpU_LE,
         CodeX = Code::CmpI_LE;
   };

   //
   // CodeSet_CmpLT
   //
   struct CodeSet_CmpLT
   {
      static constexpr Code
         CodeA = Code::CmpU_LT,
         CodeF = Code::CmpF_LT,
         CodeI = Code::CmpI_LT,
         CodeK = Code::CmpU_LT,
         CodeR = Code::CmpI_LT,
         CodeU = Code::CmpU_LT,
         CodeX = Code::CmpI_LT;
   };

   //
   // CodeSet_CmpNE
   //
   struct CodeSet_CmpNE
   {
      static constexpr Code
         CodeA = Code::CmpU_NE,
         CodeF = Code::CmpF_NE,
         CodeI = Code::CmpI_NE,
         CodeK = Code::CmpU_NE,
         CodeR = Code::CmpI_NE,
         CodeU = Code::CmpU_NE,
         CodeX = Code::CmpI_NE;
   };
}

#endif//GDCC__IR__CodeSet__Cmp_H__

