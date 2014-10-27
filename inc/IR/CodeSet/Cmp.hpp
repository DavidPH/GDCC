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
            CodeA_W  = Code::CmpU_EQ_W,
            CodeA_W2 = Code::CmpU_EQ_W2,
            CodeF_W  = Code::CmpF_EQ_W,
            CodeF_W2 = Code::CmpF_EQ_W2,
            CodeI_W  = Code::CmpI_EQ_W,
            CodeI_W2 = Code::CmpI_EQ_W2,
            CodeI_W3 = Code::CmpI_EQ_W3,
            CodeK_W  = Code::CmpU_EQ_W,
            CodeK_W2 = Code::CmpU_EQ_W2,
            CodeR_W  = Code::CmpI_EQ_W,
            CodeR_W2 = Code::CmpI_EQ_W2,
            CodeU_W  = Code::CmpU_EQ_W,
            CodeU_W2 = Code::CmpU_EQ_W2,
            CodeU_W3 = Code::CmpU_EQ_W3,
            CodeX_W  = Code::CmpI_EQ_W,
            CodeX_W2 = Code::CmpI_EQ_W2;
      };

      //
      // CodeSet_CmpGE
      //
      struct CodeSet_CmpGE
      {
         static constexpr Code
            CodeA_W  = Code::CmpU_GE_W,
            CodeA_W2 = Code::CmpU_GE_W2,
            CodeF_W  = Code::CmpF_GE_W,
            CodeF_W2 = Code::CmpF_GE_W2,
            CodeI_W  = Code::CmpI_GE_W,
            CodeI_W2 = Code::CmpI_GE_W2,
            CodeI_W3 = Code::CmpI_GE_W3,
            CodeK_W  = Code::CmpU_GE_W,
            CodeK_W2 = Code::CmpU_GE_W2,
            CodeR_W  = Code::CmpI_GE_W,
            CodeR_W2 = Code::CmpI_GE_W2,
            CodeU_W  = Code::CmpU_GE_W,
            CodeU_W2 = Code::CmpU_GE_W2,
            CodeU_W3 = Code::CmpU_GE_W3,
            CodeX_W  = Code::CmpI_GE_W,
            CodeX_W2 = Code::CmpI_GE_W2;
      };

      //
      // CodeSet_CmpGT
      //
      struct CodeSet_CmpGT
      {
         static constexpr Code
            CodeA_W  = Code::CmpU_GT_W,
            CodeA_W2 = Code::CmpU_GT_W2,
            CodeF_W  = Code::CmpF_GT_W,
            CodeF_W2 = Code::CmpF_GT_W2,
            CodeI_W  = Code::CmpI_GT_W,
            CodeI_W2 = Code::CmpI_GT_W2,
            CodeI_W3 = Code::CmpI_GT_W3,
            CodeK_W  = Code::CmpU_GT_W,
            CodeK_W2 = Code::CmpU_GT_W2,
            CodeR_W  = Code::CmpI_GT_W,
            CodeR_W2 = Code::CmpI_GT_W2,
            CodeU_W  = Code::CmpU_GT_W,
            CodeU_W2 = Code::CmpU_GT_W2,
            CodeU_W3 = Code::CmpU_GT_W3,
            CodeX_W  = Code::CmpI_GT_W,
            CodeX_W2 = Code::CmpI_GT_W2;
      };

      //
      // CodeSet_CmpLE
      //
      struct CodeSet_CmpLE
      {
         static constexpr Code
            CodeA_W  = Code::CmpU_LE_W,
            CodeA_W2 = Code::CmpU_LE_W2,
            CodeF_W  = Code::CmpF_LE_W,
            CodeF_W2 = Code::CmpF_LE_W2,
            CodeI_W  = Code::CmpI_LE_W,
            CodeI_W2 = Code::CmpI_LE_W2,
            CodeI_W3 = Code::CmpI_LE_W3,
            CodeK_W  = Code::CmpU_LE_W,
            CodeK_W2 = Code::CmpU_LE_W2,
            CodeR_W  = Code::CmpI_LE_W,
            CodeR_W2 = Code::CmpI_LE_W2,
            CodeU_W  = Code::CmpU_LE_W,
            CodeU_W2 = Code::CmpU_LE_W2,
            CodeU_W3 = Code::CmpU_LE_W3,
            CodeX_W  = Code::CmpI_LE_W,
            CodeX_W2 = Code::CmpI_LE_W2;
      };

      //
      // CodeSet_CmpLT
      //
      struct CodeSet_CmpLT
      {
         static constexpr Code
            CodeA_W  = Code::CmpU_LT_W,
            CodeA_W2 = Code::CmpU_LT_W2,
            CodeF_W  = Code::CmpF_LT_W,
            CodeF_W2 = Code::CmpF_LT_W2,
            CodeI_W  = Code::CmpI_LT_W,
            CodeI_W2 = Code::CmpI_LT_W2,
            CodeI_W3 = Code::CmpI_LT_W3,
            CodeK_W  = Code::CmpU_LT_W,
            CodeK_W2 = Code::CmpU_LT_W2,
            CodeR_W  = Code::CmpI_LT_W,
            CodeR_W2 = Code::CmpI_LT_W2,
            CodeU_W  = Code::CmpU_LT_W,
            CodeU_W2 = Code::CmpU_LT_W2,
            CodeU_W3 = Code::CmpU_LT_W3,
            CodeX_W  = Code::CmpI_LT_W,
            CodeX_W2 = Code::CmpI_LT_W2;
      };

      //
      // CodeSet_CmpNE
      //
      struct CodeSet_CmpNE
      {
         static constexpr Code
            CodeA_W  = Code::CmpU_NE_W,
            CodeA_W2 = Code::CmpU_NE_W2,
            CodeF_W  = Code::CmpF_NE_W,
            CodeF_W2 = Code::CmpF_NE_W2,
            CodeI_W  = Code::CmpI_NE_W,
            CodeI_W2 = Code::CmpI_NE_W2,
            CodeI_W3 = Code::CmpI_NE_W3,
            CodeK_W  = Code::CmpU_NE_W,
            CodeK_W2 = Code::CmpU_NE_W2,
            CodeR_W  = Code::CmpI_NE_W,
            CodeR_W2 = Code::CmpI_NE_W2,
            CodeU_W  = Code::CmpU_NE_W,
            CodeU_W2 = Code::CmpU_NE_W2,
            CodeU_W3 = Code::CmpU_NE_W3,
            CodeX_W  = Code::CmpI_NE_W,
            CodeX_W2 = Code::CmpI_NE_W2;
      };
   }
}

#endif//GDCC__IR__CodeSet__Cmp_H__

