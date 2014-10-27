//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
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

namespace GDCC
{
   namespace IR
   {
      //
      // CodeSet_Inv
      //
      struct CodeSet_Inv
      {
         static constexpr Code
            CodeI_W  = Code::InvU_W,
            CodeI_W2 = Code::InvU_W2,
            CodeI_W3 = Code::InvU_W3,
            CodeU_W  = Code::InvU_W,
            CodeU_W2 = Code::InvU_W2,
            CodeU_W3 = Code::InvU_W3;
      };

      //
      // CodeSet_Neg
      //
      struct CodeSet_Neg
      {
         static constexpr Code
            CodeA_W  = Code::NegI_W,
            CodeA_W2 = Code::NegI_W2,
            CodeF_W  = Code::NegF_W,
            CodeF_W2 = Code::NegF_W2,
            CodeI_W  = Code::NegI_W,
            CodeI_W2 = Code::NegI_W2,
            CodeI_W3 = Code::NegI_W3,
            CodeK_W  = Code::NegI_W,
            CodeK_W2 = Code::NegI_W2,
            CodeR_W  = Code::NegI_W,
            CodeR_W2 = Code::NegI_W2,
            CodeU_W  = Code::NegI_W,
            CodeU_W2 = Code::NegI_W2,
            CodeU_W3 = Code::NegI_W3,
            CodeX_W  = Code::NegI_W,
            CodeX_W2 = Code::NegI_W2;
      };
   }
}

#endif//GDCC__IR__CodeSet__Unary_H__

