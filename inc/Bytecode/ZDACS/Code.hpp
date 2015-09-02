//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2015 David Hill
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
            AddU_LocReg =  31,
            AddU_MapReg =  32,
            AddU_WldReg =  33,
            SubU_LocReg =  34,
            SubU_MapReg =  35,
            SubU_WldReg =  36,
            IncU_LocReg =  46,
            IncU_MapReg =  47,
            IncU_WldReg =  48,
            DecU_LocReg =  49,
            DecU_MapReg =  50,
            DecU_WldReg =  51,
            Jump_Lit    =  52,
            Jcnd_Tru    =  53,
            Drop_Nul    =  54,
            Wait_Lit    =  56,
            LAnd        =  70,
            LOrI        =  71,
            AndU        =  72,
            OrIU        =  73,
            OrXU        =  74,
            NotU        =  75,
            ShLU        =  76,
            ShRI        =  77,
            NegI        =  78,
            Jcnd_Nil    =  79,
            Jcnd_Lit    =  84,
            MulX        = 136,
            DivX        = 137,
            Drop_GblReg = 181,
            Push_GblReg = 182,
            AddU_GblReg = 183,
            SubU_GblReg = 184,
            IncU_GblReg = 188,
            DecU_GblReg = 189,
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
            Jcnd_Tab    = 256,
            Drop_ScrRet = 257,
            Cspe_5R1    = 263,
            InvU        = 330,
            Cnat        = 351,
            Pfun_Lit    = 359,
            Call_Stk    = 360,
            Jdyn        = 363,
            Drop_LocArr = 364,
            Push_LocArr = 365,
         };
      }
   }
}

#endif//GDCC__Bytecode__ZDACS__Code_H__

