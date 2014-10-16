//-----------------------------------------------------------------------------
//
// Copyright (C) 2014 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS instruction codes.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__Bytecode__ZDACS__Code_H__
#define GDCC__Bytecode__ZDACS__Code_H__


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC
{
   namespace Bytecode
   {
      namespace ZDACS
      {
         //
         // Code
         //
         enum class Code
         {
            Nop         =   0,
            Rscr        =   1,
            Push_Lit    =   3,
            Cspe_1      =   4,
            Cspe_2      =   5,
            Cspe_3      =   6,
            Cspe_4      =   7,
            Cspe_5      =   8,
            Cspe_1L     =   9,
            Cspe_2L     =  10,
            Cspe_3L     =  11,
            Cspe_4L     =  12,
            Cspe_5L     =  13,
            AddU        =  14,
            SubU        =  15,
            MulU        =  16,
            DivI        =  17,
            ModI        =  18,
            CmpU_EQ     =  19,
            CmpU_NE     =  20,
            CmpI_LT     =  21,
            CmpI_GT     =  22,
            CmpI_LE     =  23,
            CmpI_GE     =  24,
            Drop_LocReg =  25,
            Drop_MapReg =  26,
            Drop_WldReg =  27,
            Push_LocReg =  28,
            Push_MapReg =  29,
            Push_WldReg =  30,
            DecU_LocReg =  49,
            Jump_Lit    =  52,
            Cjmp_Tru    =  53,
            Drop_Nul    =  54,
            Wait_Lit    =  56,
            AndU        =  72,
            OrIU        =  73,
            OrXU        =  74,
            NotU        =  75,
            ShLU        =  76,
            ShRI        =  77,
            NegI        =  78,
            Cjmp_Nil    =  79,
            Cjmp_Lit    =  84,
            MulX        = 136,
            DivX        = 137,
            Drop_GblReg = 181,
            Push_GblReg = 182,
            Call_Lit    = 203,
            Call_Nul    = 204,
            Retn_Nul    = 205,
            Retn_Stk    = 206,
            Push_MapArr = 207,
            Drop_MapArr = 208,
            Copy        = 216,
            Swap        = 217,
            Pstr_Stk    = 225,
            Push_WldArr = 226,
            Drop_WldArr = 227,
            Push_GblArr = 235,
            Drop_GblArr = 236,
            Drop_ScrRet = 257,
            Cspe_5R1    = 263,
            InvU        = 330,
            Cnat        = 351,
            Pfun_Lit    = 359,
            Call_Stk    = 360,
         };
      }
   }
}

#endif//GDCC__Bytecode__ZDACS__Code_H__

