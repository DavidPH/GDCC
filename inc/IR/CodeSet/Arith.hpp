//-----------------------------------------------------------------------------
//
// Copyright (C) 2013-2014 David Hill
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

namespace GDCC
{
   namespace IR
   {
      //
      // CodeSet_Add
      //
      struct CodeSet_Add
      {
         static constexpr Code
            CodeA_W  = Code::AddU_W,
            CodeA_W2 = Code::AddU_W2,
            CodeF_W  = Code::AddF_W,
            CodeF_W2 = Code::AddF_W2,
            CodeI_W  = Code::AddI_W,
            CodeI_W2 = Code::AddI_W2,
            CodeI_W3 = Code::AddI_W3,
            CodeK_W  = Code::AddU_W,
            CodeK_W2 = Code::AddU_W2,
            CodeR_W  = Code::AddI_W,
            CodeR_W2 = Code::AddI_W2,
            CodeU_W  = Code::AddU_W,
            CodeU_W2 = Code::AddU_W2,
            CodeU_W3 = Code::AddU_W3,
            CodeX_W  = Code::AddI_W,
            CodeX_W2 = Code::AddI_W2;
      };

      //
      // CodeSet_DiX
      //
      struct CodeSet_DiX
      {
         static constexpr Code
            CodeA_W  = Code::None,
            CodeA_W2 = Code::None,
            CodeF_W  = Code::None,
            CodeF_W2 = Code::None,
            CodeI_W  = Code::DiXI_W,
            CodeI_W2 = Code::DiXI_W2,
            CodeI_W3 = Code::DiXI_W3,
            CodeK_W  = Code::None,
            CodeK_W2 = Code::None,
            CodeR_W  = Code::None,
            CodeR_W2 = Code::None,
            CodeU_W  = Code::DiXU_W,
            CodeU_W2 = Code::DiXU_W2,
            CodeU_W3 = Code::DiXU_W3,
            CodeX_W  = Code::None,
            CodeX_W2 = Code::None;
      };

      //
      // CodeSet_Div
      //
      struct CodeSet_Div
      {
         static constexpr Code
            CodeA_W  = Code::None,
            CodeA_W2 = Code::None,
            CodeF_W  = Code::DivF_W,
            CodeF_W2 = Code::DivF_W2,
            CodeI_W  = Code::DivI_W,
            CodeI_W2 = Code::DivI_W2,
            CodeI_W3 = Code::DivI_W3,
            CodeK_W  = Code::None,
            CodeK_W2 = Code::None,
            CodeR_W  = Code::None,
            CodeR_W2 = Code::None,
            CodeU_W  = Code::DivU_W,
            CodeU_W2 = Code::DivU_W2,
            CodeU_W3 = Code::DivU_W3,
            CodeX_W  = Code::DivX_W,
            CodeX_W2 = Code::DivX_W2;
      };

      //
      // CodeSet_Mod
      //
      struct CodeSet_Mod
      {
         static constexpr Code
            CodeA_W  = Code::ModU_W,
            CodeA_W2 = Code::ModU_W2,
            CodeF_W  = Code::None,
            CodeF_W2 = Code::None,
            CodeI_W  = Code::ModI_W,
            CodeI_W2 = Code::ModI_W2,
            CodeI_W3 = Code::ModI_W3,
            CodeK_W  = Code::ModU_W,
            CodeK_W2 = Code::ModU_W2,
            CodeR_W  = Code::ModI_W,
            CodeR_W2 = Code::ModI_W2,
            CodeU_W  = Code::ModU_W,
            CodeU_W2 = Code::ModU_W2,
            CodeU_W3 = Code::ModU_W3,
            CodeX_W  = Code::ModI_W,
            CodeX_W2 = Code::ModI_W2;
      };

      //
      // CodeSet_Mul
      //
      struct CodeSet_Mul
      {
         static constexpr Code
            CodeA_W  = Code::None,
            CodeA_W2 = Code::None,
            CodeF_W  = Code::MulF_W,
            CodeF_W2 = Code::MulF_W2,
            CodeI_W  = Code::MulI_W,
            CodeI_W2 = Code::MulI_W2,
            CodeI_W3 = Code::MulI_W3,
            CodeK_W  = Code::None,
            CodeK_W2 = Code::None,
            CodeR_W  = Code::None,
            CodeR_W2 = Code::None,
            CodeU_W  = Code::MulU_W,
            CodeU_W2 = Code::MulU_W2,
            CodeU_W3 = Code::MulU_W3,
            CodeX_W  = Code::MulX_W,
            CodeX_W2 = Code::MulX_W2;
      };

      //
      // CodeSet_Sub
      //
      struct CodeSet_Sub
      {
         static constexpr Code
            CodeA_W  = Code::SubU_W,
            CodeA_W2 = Code::SubU_W2,
            CodeF_W  = Code::SubF_W,
            CodeF_W2 = Code::SubF_W2,
            CodeI_W  = Code::SubI_W,
            CodeI_W2 = Code::SubI_W2,
            CodeI_W3 = Code::SubI_W3,
            CodeK_W  = Code::SubU_W,
            CodeK_W2 = Code::SubU_W2,
            CodeR_W  = Code::SubI_W,
            CodeR_W2 = Code::SubI_W2,
            CodeU_W  = Code::SubU_W,
            CodeU_W2 = Code::SubU_W2,
            CodeU_W3 = Code::SubU_W3,
            CodeX_W  = Code::SubI_W,
            CodeX_W2 = Code::SubI_W2;
      };
   }
}

#endif//GDCC__IR__CodeSet__Arith_H__

