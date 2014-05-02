//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
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

namespace GDCC
{
   namespace IR
   {
      //
      // CodeSet_CmpEQ
      //
      struct CodeSet_CmpEQ
      {
         static constexpr Code
            CodeA_W = Code::CmpU_EQ_W,
            CodeF_W = Code::None,
            CodeI_W = Code::CmpI_EQ_W,
            CodeK_W = Code::CmpU_EQ_W,
            CodeR_W = Code::CmpI_EQ_W,
            CodeU_W = Code::CmpU_EQ_W,
            CodeX_W = Code::CmpI_EQ_W;
      };

      //
      // CodeSet_CmpGE
      //
      struct CodeSet_CmpGE
      {
         static constexpr Code
            CodeA_W = Code::CmpU_GE_W,
            CodeF_W = Code::None,
            CodeI_W = Code::CmpI_GE_W,
            CodeK_W = Code::CmpU_GE_W,
            CodeR_W = Code::CmpI_GE_W,
            CodeU_W = Code::CmpU_GE_W,
            CodeX_W = Code::CmpI_GE_W;
      };

      //
      // CodeSet_CmpGT
      //
      struct CodeSet_CmpGT
      {
         static constexpr Code
            CodeA_W = Code::CmpU_GT_W,
            CodeF_W = Code::None,
            CodeI_W = Code::CmpI_GT_W,
            CodeK_W = Code::CmpU_GT_W,
            CodeR_W = Code::CmpI_GT_W,
            CodeU_W = Code::CmpU_GT_W,
            CodeX_W = Code::CmpI_GT_W;
      };

      //
      // CodeSet_CmpLE
      //
      struct CodeSet_CmpLE
      {
         static constexpr Code
            CodeA_W = Code::CmpU_LE_W,
            CodeF_W = Code::None,
            CodeI_W = Code::CmpI_LE_W,
            CodeK_W = Code::CmpU_LE_W,
            CodeR_W = Code::CmpI_LE_W,
            CodeU_W = Code::CmpU_LE_W,
            CodeX_W = Code::CmpI_LE_W;
      };

      //
      // CodeSet_CmpLT
      //
      struct CodeSet_CmpLT
      {
         static constexpr Code
            CodeA_W = Code::CmpU_LT_W,
            CodeF_W = Code::None,
            CodeI_W = Code::CmpI_LT_W,
            CodeK_W = Code::CmpU_LT_W,
            CodeR_W = Code::CmpI_LT_W,
            CodeU_W = Code::CmpU_LT_W,
            CodeX_W = Code::CmpI_LT_W;
      };

      //
      // CodeSet_CmpNE
      //
      struct CodeSet_CmpNE
      {
         static constexpr Code
            CodeA_W = Code::CmpU_NE_W,
            CodeF_W = Code::None,
            CodeI_W = Code::CmpI_NE_W,
            CodeK_W = Code::CmpU_NE_W,
            CodeR_W = Code::CmpI_NE_W,
            CodeU_W = Code::CmpU_NE_W,
            CodeX_W = Code::CmpI_NE_W;
      };
   }
}

#endif//GDCC__IR__CodeSet__Cmp_H__
