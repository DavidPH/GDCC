//-----------------------------------------------------------------------------
//
// Copyright (C) 2014-2024 David Hill
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// ZDoom ACS instruction codes.
//
//-----------------------------------------------------------------------------

#ifndef GDCC__BC__ZDACS__Code_H__
#define GDCC__BC__ZDACS__Code_H__

#include "../../BC/ZDACS/Types.hpp"


//----------------------------------------------------------------------------|
// Types                                                                      |
//

namespace GDCC::BC::ZDACS
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
      Drop_ModReg =  26,
      Drop_HubReg =  27,
      Push_LocReg =  28,
      Push_ModReg =  29,
      Push_HubReg =  30,
      AddU_LocReg =  31,
      AddU_ModReg =  32,
      AddU_HubReg =  33,
      SubU_LocReg =  34,
      SubU_ModReg =  35,
      SubU_HubReg =  36,
      IncU_LocReg =  46,
      IncU_ModReg =  47,
      IncU_HubReg =  48,
      DecU_LocReg =  49,
      DecU_ModReg =  50,
      DecU_HubReg =  51,
      Jump_Lit    =  52,
      Jcnd_Tru    =  53,
      Drop_Nul    =  54,
      Wait_Lit    =  56,
      LAnd        =  70,
      LOrI        =  71,
      BAnd        =  72,
      BOrI        =  73,
      BOrX        =  74,
      LNot        =  75,
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
      Push_ModArr = 207,
      Drop_ModArr = 208,
      IncU_ModArr = 214,
      DecU_ModArr = 215,
      Copy        = 216,
      Swap        = 217,
      Pstr_Stk    = 225,
      Push_HubArr = 226,
      Drop_HubArr = 227,
      IncU_HubArr = 233,
      DecU_HubArr = 234,
      Push_GblArr = 235,
      Drop_GblArr = 236,
      IncU_GblArr = 242,
      DecU_GblArr = 243,
      Jcnd_Tab    = 256,
      Drop_ScrRet = 257,
      Cspe_5R1    = 263,
      BNot        = 330,
      Cnat        = 351,
      Pfun_Lit    = 359,
      Call_Stk    = 360,
      Jdyn        = 363,
      Drop_LocArr = 364,
      Push_LocArr = 365,
      IncU_LocArr = 371,
      DecU_LocArr = 372,
   };
}

#endif//GDCC__BC__ZDACS__Code_H__

