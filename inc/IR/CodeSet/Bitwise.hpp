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
            CodeI_W = Code::AndU_W,
            CodeU_W = Code::AndU_W;
      };

      //
      // CodeSet_OrI
      //
      struct CodeSet_OrI
      {
         static constexpr Code
            CodeI_W = Code::OrIU_W,
            CodeU_W = Code::OrIU_W;
      };

      //
      // CodeSet_OrX
      //
      struct CodeSet_OrX
      {
         static constexpr Code
            CodeI_W = Code::OrXU_W,
            CodeU_W = Code::OrXU_W;
      };

      //
      // CodeSet_ShL
      //
      struct CodeSet_ShL
      {
         static constexpr Code
            CodeA_W = Code::ShLU_W,
            CodeF_W = Code::ShLF_W,
            CodeI_W = Code::ShLU_W,
            CodeK_W = Code::ShLU_W,
            CodeR_W = Code::ShLU_W,
            CodeU_W = Code::ShLU_W,
            CodeX_W = Code::ShLU_W;
      };

      //
      // CodeSet_ShR
      //
      struct CodeSet_ShR
      {
         static constexpr Code
            CodeA_W = Code::ShRU_W,
            CodeF_W = Code::ShRF_W,
            CodeI_W = Code::ShRI_W,
            CodeK_W = Code::ShRU_W,
            CodeR_W = Code::ShRI_W,
            CodeU_W = Code::ShRU_W,
            CodeX_W = Code::ShRI_W;
      };
   }
}

#endif//GDCC__IR__CodeSet__Bitwise_H__

