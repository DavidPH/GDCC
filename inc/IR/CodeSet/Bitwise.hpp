//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
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

namespace GDCC
{
   namespace IR
   {
      //
      // CodeSet_And
      //
      struct CodeSet_And
      {
         static constexpr Code
            CodeI_W  = Code::AndU_W,
            CodeI_W2 = Code::AndU_W2,
            CodeI_W3 = Code::AndU_W3,
            CodeU_W  = Code::AndU_W,
            CodeU_W2 = Code::AndU_W2,
            CodeU_W3 = Code::AndU_W3;
      };

      //
      // CodeSet_OrI
      //
      struct CodeSet_OrI
      {
         static constexpr Code
            CodeI_W  = Code::OrIU_W,
            CodeI_W2 = Code::OrIU_W2,
            CodeI_W3 = Code::OrIU_W3,
            CodeU_W  = Code::OrIU_W,
            CodeU_W2 = Code::OrIU_W2,
            CodeU_W3 = Code::OrIU_W3;
      };

      //
      // CodeSet_OrX
      //
      struct CodeSet_OrX
      {
         static constexpr Code
            CodeI_W  = Code::OrXU_W,
            CodeI_W2 = Code::OrXU_W2,
            CodeI_W3 = Code::OrXU_W3,
            CodeU_W  = Code::OrXU_W,
            CodeU_W2 = Code::OrXU_W2,
            CodeU_W3 = Code::OrXU_W3;
      };

      //
      // CodeSet_ShL
      //
      struct CodeSet_ShL
      {
         static constexpr Code
            CodeA_W  = Code::ShLU_W,
            CodeA_W2 = Code::ShLU_W2,
            CodeI_W  = Code::ShLU_W,
            CodeI_W2 = Code::ShLU_W2,
            CodeI_W3 = Code::ShLU_W3,
            CodeK_W  = Code::ShLU_W,
            CodeK_W2 = Code::ShLU_W2,
            CodeR_W  = Code::ShLU_W,
            CodeR_W2 = Code::ShLU_W2,
            CodeU_W  = Code::ShLU_W,
            CodeU_W2 = Code::ShLU_W2,
            CodeU_W3 = Code::ShLU_W3,
            CodeX_W  = Code::ShLU_W,
            CodeX_W2 = Code::ShLU_W2;
      };

      //
      // CodeSet_ShR
      //
      struct CodeSet_ShR
      {
         static constexpr Code
            CodeA_W  = Code::ShRU_W,
            CodeA_W2 = Code::ShRU_W2,
            CodeI_W  = Code::ShRI_W,
            CodeI_W2 = Code::ShRI_W2,
            CodeI_W3 = Code::ShRI_W3,
            CodeK_W  = Code::ShRU_W,
            CodeK_W2 = Code::ShRU_W2,
            CodeR_W  = Code::ShRI_W,
            CodeR_W2 = Code::ShRI_W2,
            CodeU_W  = Code::ShRU_W,
            CodeU_W2 = Code::ShRU_W2,
            CodeU_W3 = Code::ShRU_W3,
            CodeX_W  = Code::ShRI_W,
            CodeX_W2 = Code::ShRI_W2;
      };
   }
}

#endif//GDCC__IR__CodeSet__Bitwise_H__

