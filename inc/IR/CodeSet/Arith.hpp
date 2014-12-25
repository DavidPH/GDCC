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
            CodeA_W = Code::AddU_W,
            CodeF_W = Code::AddF_W,
            CodeI_W = Code::AddI_W,
            CodeK_W = Code::AddU_W,
            CodeR_W = Code::AddI_W,
            CodeU_W = Code::AddU_W,
            CodeX_W = Code::AddI_W;
      };

      //
      // CodeSet_DiX
      //
      struct CodeSet_DiX
      {
         static constexpr Code
            CodeA_W = Code::None,
            CodeF_W = Code::None,
            CodeI_W = Code::DiXI_W,
            CodeK_W = Code::None,
            CodeR_W = Code::None,
            CodeU_W = Code::DiXU_W,
            CodeX_W = Code::None;
      };

      //
      // CodeSet_Div
      //
      struct CodeSet_Div
      {
         static constexpr Code
            CodeA_W = Code::None,
            CodeF_W = Code::DivF_W,
            CodeI_W = Code::DivI_W,
            CodeK_W = Code::DivK_W,
            CodeR_W = Code::None,
            CodeU_W = Code::DivU_W,
            CodeX_W = Code::DivX_W;
      };

      //
      // CodeSet_Mod
      //
      struct CodeSet_Mod
      {
         static constexpr Code
            CodeA_W = Code::ModU_W,
            CodeF_W = Code::None,
            CodeI_W = Code::ModI_W,
            CodeK_W = Code::ModU_W,
            CodeR_W = Code::ModI_W,
            CodeU_W = Code::ModU_W,
            CodeX_W = Code::ModI_W;
      };

      //
      // CodeSet_Mul
      //
      struct CodeSet_Mul
      {
         static constexpr Code
            CodeA_W = Code::None,
            CodeF_W = Code::MulF_W,
            CodeI_W = Code::MulI_W,
            CodeK_W = Code::MulK_W,
            CodeR_W = Code::None,
            CodeU_W = Code::MulU_W,
            CodeX_W = Code::MulX_W;
      };

      //
      // CodeSet_Sub
      //
      struct CodeSet_Sub
      {
         static constexpr Code
            CodeA_W = Code::SubU_W,
            CodeF_W = Code::SubF_W,
            CodeI_W = Code::SubI_W,
            CodeK_W = Code::SubU_W,
            CodeR_W = Code::SubI_W,
            CodeU_W = Code::SubU_W,
            CodeX_W = Code::SubI_W;
      };
   }
}

#endif//GDCC__IR__CodeSet__Arith_H__

